//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
//
//This file is part of HappyEngine.
//
//    HappyEngine is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyEngine is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyEngine.  If not, see <http://www.gnu.org/licenses/>.
//
//Author:  Bastian Damman
//Created: 24/08/2011
#include "StdAfx.h" 

#include "Server.h"
#include "HappyNew.h"
#include "Assert.h"
#include "boost/bind.hpp"
#include "HappyEngine.h"
#include "NetworkManager.h"

namespace he {
namespace net {

Server::Server(): m_pUdpSocket(nullptr), m_ReceivedBytes(0), m_SendBytes(0)
{
    m_pBuffer = malloc(MAX_MESSAGE_SIZE + sizeof(Header));
}


Server::~Server()
{
    free(m_pBuffer);
    if (m_pUdpSocket != nullptr)
    {
        stop();
    }
}

void Server::disconnectUser(byte userId)
{
    Server::Header header;
    header.type = ServerMessage_Disconnect;
    header.user = userId;

    details::Message::pointer msg(details::Message::createServerMsg(0, 0, &header, sizeof(Header)));

    userSendMessageToUser(msg->getMsg(), msg->getSizeInBytes(), 0, userId);
}
void Server::stop()
{
    if (m_ConnectedUsers.size() > 0)
    {
        std::for_each(m_ConnectedUsers.cbegin(), m_ConnectedUsers.cend(), [&](byte userId)
        {
            Server::Header header;
            header.type = ServerMessage_Disconnect;
            header.user = userId;

            details::Message::pointer msg(details::Message::createServerMsg(0, 0, &header, sizeof(Header)));

            m_pUdpSocket->send_to(boost::asio::buffer(msg->getMsg(), msg->getSizeInBytes()),
            m_Users[userId].endpoint);
        });
    }

    m_pUdpSocket->shutdown(boost::asio::socket_base::shutdown_both);
    m_pUdpSocket->close();
    delete m_pUdpSocket;
    m_pUdpSocket = nullptr;
    std::cout << "Connection closed.\n";
    std::cout << "Send " << m_SendBytes / 1024.0f / 1024.0f << "MB" << "\n";
    std::cout << "Received " << m_ReceivedBytes / 1024.0f / 1024.0f << "MB" << "\n";
}
void Server::start(ushort port, byte maxConnections)
{
    ASSERT(m_pUdpSocket == nullptr, "m_pUdpSocket is allready initialized");

    m_pUdpSocket = NEW boost::asio::ip::udp::socket(
        NETWORK->getIoService(), 
        boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port));

    m_ConnectedUsers.clear();
    m_ConnectedUsers.reserve(maxConnections);
    m_Users.clear();
    m_Users.resize(maxConnections);

    m_FreeSlots = std::queue<byte>();
    for (byte i = 0; i < maxConnections; ++i)
    {
        m_FreeSlots.push(i);
    }

    std::cout << "Connection opened: " << m_pUdpSocket->local_endpoint().address().to_string() <<
        ":" << (int)m_pUdpSocket->local_endpoint().port() << "\n";

    asycRead();
}
void Server::asycRead()
{
    m_pUdpSocket->async_receive_from(boost::asio::buffer(m_pBuffer, MAX_MESSAGE_SIZE), 
        m_LastPacketSender,
        boost::bind(&Server::handleReceive, this, boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}
void Server::handleReceive(const boost::system::error_code& error, size_t bytesReceived)
{
    m_ReceivedBytes += bytesReceived;
    if (!error)
    {
        Header* msg(static_cast<Header*>(m_pBuffer));
        if (msg->type == ServerMessage_None)
        {
            handleClientMessage(msg+1, bytesReceived - sizeof(Header), msg->user);
        }
        else
        {
            handleServerMessage(msg+1, bytesReceived - sizeof(Header), msg);
        }
        asycRead();
    }
    else if (error != boost::asio::error::eof)
    {
        std::cout << "Server receive err\n";
    }
    else
    {
        return;
    }
}

void Server::handleServerMessage(void* /*raw_msg*/, size_t /*msg_size*/, Header* pHeader)
{
    switch (pHeader->type)
    {
        case ServerMessage_None: ASSERT("should be handled in previous method"); break;
        case ServerMessage_Connect: userConnecting(); break;
        case ServerMessage_Disconnect: userDisconnecting(pHeader->user); break;
        default: std::cout << "unkown server message received: " << pHeader->type << "\n";
    }
}
void Server::userConnecting()
{
    if (m_FreeSlots.empty() == false)
    {
        byte slot(m_FreeSlots.front());
        m_FreeSlots.pop();

        if (m_Users[slot].connected == true)
            std::cout << "warning force connecting over connected user!\n";
        m_Users[slot] = User(slot, m_LastPacketSender);
        m_ConnectedUsers.push_back(slot);
        
        Header header;
        header.type = ServerMessage_Connect;
        header.user = slot;

        byte msg = 0;

        sendMessageToUser(details::Message::createServerMsg(&msg, sizeof(byte), &header, sizeof(Header)), slot);

        std::cout << "user: " << (int)slot << " connected! ("<< m_LastPacketSender.address().to_string() << ":" << 
            (int)m_LastPacketSender.port() << ")\n";
    }
    else
    {
        std::cout << "failed to connect ("<< m_LastPacketSender.address().to_string() << ":" << 
            (int)m_LastPacketSender.port() << "): to many users\n";
        
        Header header;
        header.type = ServerMessage_Connect;
        header.user = 0;

        bool msg = false;

        sendMessageToCurrent(details::Message::createServerMsg(&msg, sizeof(bool), &header, sizeof(Header)));
    }
}
void Server::userDisconnecting(byte userId)
{
    ASSERT(userId < m_Users.size(), "userID > than allowed users");
    if (m_Users[userId].connected == true)
    {
        handleUserDisconnecting(userId);
        m_Users[userId].connected = false;
        m_FreeSlots.push(userId);
        m_ConnectedUsers.erase(std::remove(m_ConnectedUsers.begin(), m_ConnectedUsers.end(), userId), m_ConnectedUsers.end());
        std::cout << "User " << (int)userId << " disconnected!\n";
    }
    else
        std::cout << "warning disconnecting disconnected user!\n";
}
void Server::handleUserDisconnecting(byte /*userId*/)
{
}

void Server::userSendMessageToAll(const void* pMsg, uint sizeInBytes, byte from)
{
    Header header;
    header.type = ServerMessage_None;
    header.user = from;
    sendMessageToAll(details::Message::createServerMsg(pMsg, sizeInBytes, &header, sizeof(Header)));
}
void Server::userSendMessageToAllBut(const void* pMsg, uint sizeInBytes, byte from, byte userId)
{
    Header header;
    header.type = ServerMessage_None;
    header.user = from;
    sendMessageToAllBut(details::Message::createServerMsg(pMsg, sizeInBytes, &header, sizeof(Header)), userId);
}
void Server::userSendMessageToUser(const void* pMsg, uint sizeInBytes, byte from, byte userId)
{
    Header header;
    header.type = ServerMessage_None;
    header.user = from;
    sendMessageToUser(details::Message::createServerMsg(pMsg, sizeInBytes, &header, sizeof(Header)), userId);
}

void Server::sendMessageToAll(const details::Message::pointer& msg)
{
    std::for_each(m_ConnectedUsers.cbegin(), m_ConnectedUsers.cend(), [&](byte userId)
    {
        sendMessageToUser(msg, userId);
    });
}
void Server::sendMessageToAllBut(const details::Message::pointer& msg, byte butUserId)
{
    std::for_each(m_ConnectedUsers.cbegin(), m_ConnectedUsers.cend(), [&](byte userId)
    {
        if (userId != butUserId)
            sendMessageToUser(msg, userId);
    });
}
void Server::sendMessageToUser(const details::Message::pointer& msg, byte userId)
{
    m_pUdpSocket->async_send_to(boost::asio::buffer(msg->getMsg(), msg->getSizeInBytes()), 
        m_Users[userId].endpoint,
        boost::bind(&Server::handleWrite, this, 
                    msg, userId,
                    boost::asio::placeholders::error, 
                    boost::asio::placeholders::bytes_transferred));
}
void Server::sendMessageToCurrent(const details::Message::pointer& msg)
{
    m_pUdpSocket->async_send_to(boost::asio::buffer(msg->getMsg(), msg->getSizeInBytes()), 
        m_LastPacketSender,
        boost::bind(&Server::handleWrite, this,
                    msg, (byte)0,
                    boost::asio::placeholders::error, 
                    boost::asio::placeholders::bytes_transferred));
}

void Server::handleWrite(
    details::Message::pointer /*msg*/, byte /*userId*/,
    const boost::system::error_code& error, size_t bytesWritten)
{
    m_SendBytes += bytesWritten;
    if (!error)
    {

    }
    else if (error != boost::asio::error::eof)
    {
        std::cout << "err while sending message\n";
    }
    else
    {
        return;
    }
}
const std::vector<byte>& Server::getConnectedUsers() const
{
    return m_ConnectedUsers;
}


} } //end namespace