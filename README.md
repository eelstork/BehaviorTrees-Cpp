# BehaviorTrees-Cpp

C++ [behavior trees](https://en.wikipedia.org/wiki/Behavior_tree_(artificial_intelligence,_robotics_and_control)) via [active logic](https://arxiv.org/ftp/arxiv/papers/2011/2011.03835.pdf)

Behavior trees simplify responsive, discrete control applications (such as with tick/update functions), including networking, video games, dynamic UIs,
robotics and cognitive modeling; tight integration enables performance with direct support for functional and object programming.

*Grazer.cpp*
```cpp
// GCC: compile with -fno-operator-names 

#include "status-pexp.h"
#include "Grazer.h"

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
```

## How it works

[status](BehaviorTrees/status.h) is a three valued struct. A status may be *complete*, *running* or *failing*.

The library defines 6 operators; three operators support sequential execution:

- the `and` operator is used to sequence subtasks; the second task (rh) executes if the first (lh) is complete
- the `or` operator, aka 'selector' is used to prioritize subtasks; rh executes if lh is failing
- the `over` operator is used to prioritize work (or waiting on an external task); rh executes if lh is failing or complete

Three operators support parallel execution:

- with `por` (parallel-or), rh executes if lh is not complete
- with `pand` (parallel-and), rh executes if lh is not failing
- the `with` operator is used to strictly parallelize; in this case, rh will only execute if lh is running

Classic behavior trees mainly use sequences and selectors. When tasks are viewed as *activities*, `over` is useful
because you need not differentiate the fail/complete status of underlying subtasks:

```cpp
// Safety first! However when safety is not actionable, work proceeds; 
// resting okay, be work either complete, or failing.
status GoodJob() task( 
    Safety() over Work() over Rest()
);
```

Parallelism is useful when approaching tasks from several angles or handling multimedia applications; coordinating multiple agents does not require special operators:

```cpp
status Hunt(Agent hunters[], int count, Agent* target) {
    for (int i = 0; i < count; i++) {
        if (hunters[i].Chase(target).complete()) {
            return done;
        }
    }
    return cont;
}
```

If you do not need parallel operation, only include `status-exp.h`.

## Usage notes

a *task* is a functional block returning a status; the following is valid:

```cpp
status Graze() {
    Agent* g = ap -> Find("Grass");
    task(hungry and Consume(g) or Roam());
);
```

a *substask* is an expression returning status:

```cpp
status Step() task( 
    Flee(ap.threat) 
    or subtask( actor.Strike(other) or actor.Face(other) ) 
    or Graze() 
);
```

NOTE: `subtask(EXP)` translates to `[=] task(EXP) ()`; not recommended for high performance and you do not
need this here (associativity).

## FAQ

### What does 'stateless' mean? Do long running tasks not need tracking?

Long running tasks need tracking, which is commonly handled through executors. When you play a sound, or an animation, you
need not 'tick' the underlying service (also: while your agent are busy doing a task, turn the BT off).

In problem solving, tasks cause *motion*. As a task, filling a pool does not complete because 128 minutes have elapsed, it is complete
when the pool is full.

### Is the BT not storing current state between ticks? 

No. 

One advantage of standard behavior trees is resilience. Resilience means that if, say, a prior task has become undone (such as somebody
using your hot water to make soup while you intended to brew coffee), we just iterate the task and fix the situation.

There is a utilitarian twist to this (and a learning curve).
