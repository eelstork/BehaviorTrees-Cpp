#pragma once
#include "status.h"

using namespace activelogic;

struct Feel {

public:
    bool hungry;

};

struct Ap {

public:
    void* threat;
    void* food;
};

struct Actor {

public:

    status Strike  (void* arg)       { return fail; }
    status Face    (void* arg)       { return fail; }
    status Utter   (const char* arg) { return fail; }
    status Consume (void* arg)       { return fail; }
    status Evade   (void* arg)       { return fail; }
    status Roam    ()                { return fail; }

};