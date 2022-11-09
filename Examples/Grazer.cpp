#include "Grazer.h"
#include "status-pexp.h"

using namespace activelogic;

// 'or' expresses BT selector/fallback 
status Grazer::Step() task(
    Flee(ap.threat) or Attack(ap.threat) or Graze()
);

status Grazer::Attack(void* other) task(
    actor.Strike(other) or actor.Face(other)
);

// 'with' parallelizes subtasks
status Grazer::Flee(void* other) task(
    actor.Evade(other) with actor.Utter("Moo")
);

// 'and' expresses BT sequences 
status Grazer::Graze() task(
    feel.hungry and actor.Consume(ap.food) or actor.Roam()
);