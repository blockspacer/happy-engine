//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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
//Created: 23/06/2012
#include "HappyPCH.h" 

#include "NetworkManager.h"

#include "RakPeerInterface.h"
#include "RakPeer.h"
#include "MessageIdentifiers.h"
#include "NetworkObjectFactoryManager.h"
#include "NetworkReplicaConnection.h"
#include "NetworkIdManager.h"

namespace he {
namespace net {
    namespace details {
    void* RakMalloc(const size_t size) { return gMemMan->alloc(size MEM_DEBUG_PARAM("RakMalloc") GET_MEM_DEBUG_FL_PARAM); }
    void* RakRealloc(void *p, const size_t size) { return gMemMan->realloc(p, size MEM_DEBUG_PARAM("RakMalloc") GET_MEM_DEBUG_FL_PARAM); }
    void RakFree(void *p) { gMemMan->free(p); }
    void* RakMalloc_Ex(const size_t size, const char *file, unsigned int line) { file; line; return gMemMan->alloc(size MEM_DEBUG_PARAM("RakMalloc") PASS_MEM_DEBUG_FL_PARAMS); }
    void* RakRealloc_Ex(void *p, const size_t size, const char *file, unsigned int line) { file; line; return gMemMan->realloc(p, size MEM_DEBUG_PARAM("RakMalloc") PASS_MEM_DEBUG_FL_PARAMS); }
    void RakFree_Ex(void *p, const char* /*file*/, unsigned int /*line*/) { gMemMan->free(p); }
}

void NetworkManager::sdmInit()
{
    SetMalloc(details::RakMalloc);
    SetRealloc(details::RakRealloc);
    SetFree(details::RakFree);
    SetMalloc_Ex(details::RakMalloc_Ex);
    SetRealloc_Ex(details::RakRealloc_Ex);
    SetFree_Ex(details::RakFree_Ex);
}

void NetworkManager::sdmDestroy()
{

}

bool packageReceivedEventCombiner(bool& inoutA, const bool& inB)
{
    inoutA = inB;
    return inB;
}

NetworkManager::NetworkManager()
    : m_RakPeer(nullptr)
    , m_Sleep(0.0f)
    , m_ConnectionType(ConnectionType_Client)
    , m_NetworkObjectFactoryManager(HENew(NetworkObjectFactoryManager)())
    , m_MaxConnections(8)
    , m_NetworkIdManager(HENew(RakNet::NetworkIDManager)())
    , m_SleepTimout(1 / 30.0f)
    , PacketReceived(&packageReceivedEventCombiner, false)
{
    SetNetworkIDManager(m_NetworkIdManager);
}


NetworkManager::~NetworkManager()
{
    HEDelete(m_NetworkObjectFactoryManager);
    HEDelete(m_NetworkIdManager);
    const bool isConnected(this->isConnected());
    if (isConnected == true)
        disconnect();
}

void NetworkManager::host(uint16 port)
{
    HE_IF_ASSERT(m_RakPeer == nullptr, "Call host or join only once! Or call disconnect first!")
    {
        RakNet::SocketDescriptor sd;
        sd.socketFamily = AF_INET; //ipv4
        sd.port = port;

        m_RakPeer = RakNet::RakPeerInterface::GetInstance();

        m_RakPeer->Startup(m_MaxConnections, &sd, 1);
        m_RakPeer->SetMaximumIncomingConnections(m_MaxConnections);
        m_RakPeer->AttachPlugin(this);
        HE_INFO("Net: Starting server @ %s:%d...", "localhost", port);

        m_ConnectionType = ConnectionType_Host;

    }
}

void NetworkManager::join(const he::String& ip, uint16 port)
{
    HE_IF_ASSERT(m_RakPeer == nullptr, "Call host or join only once! Or call disconnect first!")
    {
        RakNet::SocketDescriptor sd;
        sd.socketFamily = AF_INET; //ipv4
        sd.port = 0;

        m_RakPeer = RakNet::RakPeerInterface::GetInstance();

        m_RakPeer->Startup(m_MaxConnections, &sd, 1);
        m_RakPeer->SetMaximumIncomingConnections(m_MaxConnections);
        m_RakPeer->Connect(ip.c_str(), port, 0, 0);
        m_RakPeer->AttachPlugin(this);
        HE_INFO("Net: Connecting to %s:%d...", ip.c_str(), port);

        m_ConnectionType = ConnectionType_Client;
    }
}

void NetworkManager::disconnect()
{
    HE_IF_ASSERT(isConnected(), "Already disconnected or never had a connect attempt!")
    {
        m_ServerID = UNASSIGNED_NETWORKID;
        m_RakPeer->Shutdown(100, 0);
        RakNet::RakPeerInterface::DestroyInstance(m_RakPeer);
        m_RakPeer = nullptr;
        HE_INFO("Net: disconnected");
    }
}
void NetworkManager::clientDisconnected( const NetworkID& id )
{
    m_Connections.remove(id);
    ClientDisconnected(id);
}
void NetworkManager::clientConnected( const NetworkID& id, const he::String& adress )
{
    NetworkConnection connection;
    connection.m_NetworkId = id;
    connection.m_SystemAdress = adress;
    m_Connections[id] = connection;
    ClientConnected(id);
}

void NetworkManager::tick( float dTime )
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    if (m_RakPeer == nullptr)
        return;
    m_Sleep += dTime;
    if (m_Sleep > m_SleepTimout)
    {
        m_Sleep = 0.0f; // don't care if m_Sleep is x * m_SleepTimout
        for (RakNet::Packet* packet(m_RakPeer->Receive());   packet != nullptr; 
                m_RakPeer->DeallocatePacket(packet), packet = m_RakPeer->Receive())
        {
            switch (packet->data[0])
            {
            case ID_CONNECTION_ATTEMPT_FAILED: // client
                HE_WARNING("Net: Connection attempt failed!");
                disconnect();
                ConnectionFailed();
                return;
                break;
            case ID_NO_FREE_INCOMING_CONNECTIONS: // client
                HE_WARNING("Net: Connection failed, no free incoming connections!");
                disconnect();
                ConnectionFailed();
                return;
                break;
            case ID_CONNECTION_REQUEST_ACCEPTED: // client
                HE_INFO("Net: Connection accepted!");
                m_ServerID = packet->guid;
                ConnectionSuccessful();
                break;
            case ID_NEW_INCOMING_CONNECTION: // server
                HE_INFO("Net: new connection from %s", packet->systemAddress.ToString());
                clientConnected(packet->guid, packet->systemAddress.ToString());
                break;
            case ID_DISCONNECTION_NOTIFICATION:
                if (m_ConnectionType == ConnectionType_Host) // server
                {
                    HE_INFO("Net: client disconnected: %s", packet->systemAddress.ToString());
                    clientDisconnected(packet->guid);
                }
                else // client
                {
                    HE_INFO("Net: Server has been shutdown");
                    disconnect();
                    ConnectionLost();
                    return;
                }
                break;
            case ID_CONNECTION_LOST: // server
                HE_INFO("Net: client connection lost: %s", packet->systemAddress.ToString());
                clientDisconnected(packet->guid);
                break;
            case ID_ADVERTISE_SYSTEM:
                if (m_RakPeer->GetSystemAddressFromGuid(packet->guid) == RakNet::UNASSIGNED_SYSTEM_ADDRESS && // Check if not already connected
                    m_RakPeer->GetMyGUID() != packet->guid) // Check if not connecting to ourself
                {
                    HE_INFO("Net: Connecting to %s\n", packet->systemAddress.ToString(true));
                    m_RakPeer->Connect(packet->systemAddress.ToString(false), packet->systemAddress.GetPort(), 0, 0);
                }
                break;
            case ID_SND_RECEIPT_LOSS:
            case ID_SND_RECEIPT_ACKED:
                {
                    /*uint32_t msgNumber;
                    memcpy(&msgNumber, packet->data+1, 4);

                    DataStructures::List<Replica3*> replicaListOut;
                    replicaManager.GetReplicasCreatedByMe(replicaListOut);
                    unsigned int idx;
                    for (idx=0; idx < replicaListOut.Size(); idx++)
                    {
                        ((SampleReplica*)replicaListOut[idx])->NotifyReplicaOfMessageDeliveryStatus(packet->guid,msgNumber, packet->data[0]==ID_SND_RECEIPT_ACKED);
                    }*/
                }
                break;
            default:
                {
                    if (packet->data[0] >= ID_USER_PACKET_ENUM)
                    {
                        HE_INFO("Received packet %d form %s", packet->data[0] - ID_USER_PACKET_ENUM, packet->guid.ToString());
                        m_NetworkPackage.init(packet->data, packet->length, packet->guid);
                        const bool eaten(PacketReceived(m_NetworkPackage));
                        HE_ASSERT(eaten == true, "Unhandled packet received! %d", packet->data[0] - ID_USER_PACKET_ENUM); eaten;
                        m_NetworkPackage.finalise();
                    }
                }
                break;
            }
        }
    }
}

bool NetworkManager::isConnected() const
{
    return m_RakPeer != nullptr;
}

RakNet::Connection_RM3* NetworkManager::AllocConnection( const RakNet::SystemAddress &systemAddress, RakNet::RakNetGUID rakNetGUID ) const
{
    return HENew(NetworkReplicaConnection)(systemAddress, rakNetGUID);
}

void NetworkManager::DeallocConnection( RakNet::Connection_RM3 *connection ) const
{
    HEDelete(connection);
}

bool NetworkManager::IsHost() const
{
    return m_ConnectionType == ConnectionType_Host;
}

NetworkID NetworkManager::getNetworkId() const
{
    if (m_RakPeer != nullptr)
        return m_RakPeer->GetMyGUID();
    else
        return UNASSIGNED_NETWORKID;
}

void NetworkManager::setMaxConnections( uint8 count )
{
    m_MaxConnections = count;
}

void NetworkManager::registerFactory( INetworkObjectFactory* factory )
{
    m_NetworkObjectFactoryManager->registerFactory(factory);
}

void NetworkManager::setSyncTimeout( float seconds )
{
    m_SleepTimout = seconds;
    SetAutoSerializeInterval(static_cast<RakNet::Time>(seconds * 1000 - 1));
}

void NetworkManager::send( const NetworkPackage& package, const NetworkID& to, 
        const ENetworkReliability reliability, const ENetworkPriority priority )
{
    m_RakPeer->Send(static_cast<const char*>(package.getData()), package.getByteCount(), 
        checked_numcast<PacketPriority>(priority), checked_numcast<PacketReliability>(reliability), 0, to, false);
}

void NetworkManager::sendToServer( const NetworkPackage& package, const ENetworkReliability reliability /*= eNetworkReliability_ReliableOrdered*/, const ENetworkPriority priority /*= eNetworkPriority_High*/ )
{
    HE_ASSERT(IsHost() == false, "Server can't send to server!");
    HE_ASSERT(m_ServerID != UNASSIGNED_NETWORKID, "Use of unassigned server id!");
    m_RakPeer->Send(static_cast<const char*>(package.getData()), package.getByteCount(), 
        checked_numcast<PacketPriority>(priority), checked_numcast<PacketReliability>(reliability), 0, m_ServerID, false);
}
void NetworkManager::broadcast( const NetworkPackage& package, const bool ignoreSelf, 
    const ENetworkReliability reliability, const ENetworkPriority priority )
{
    m_RakPeer->Send(static_cast<const char*>(package.getData()), package.getByteCount(), 
        checked_numcast<PacketPriority>(priority), checked_numcast<PacketReliability>(reliability), 
        0, ignoreSelf?getNetworkId():UNASSIGNED_NETWORKID, true);
}



} } //end namespace