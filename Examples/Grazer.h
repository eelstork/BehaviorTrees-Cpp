#pragma once
#include "status.h"
#include "Grazer-Defs.h"

using namespace activelogic;

class Grazer {

    Feel feel;
    Ap ap;
    Actor actor;

    status Step();

    status Attack(void* other);

    status Flee(void* other);

    status Graze();

};