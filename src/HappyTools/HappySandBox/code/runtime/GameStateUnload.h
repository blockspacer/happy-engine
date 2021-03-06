#ifndef _HS_GameStateUnload_H_
#define _HS_GameStateUnload_H_
#pragma once

#include "system/GameState.h"

namespace hs {

class GameStateUnload : public GameState
{
public:
    GameStateUnload();
    virtual ~GameStateUnload();

    virtual bool enter();
    virtual void exit();

    virtual void tick(const float dTime);

private:

    //Disable default copy constructor and default assignment operator
    GameStateUnload(const GameStateUnload&);
    GameStateUnload& operator=(const GameStateUnload&);
};

} //end namespace

#endif
