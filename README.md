# BehaviorTrees-Cpp

A C++ implementation of [behavior trees]() via [Active Logic]()

```
#include "xactive_logic.h"  // active_logic.h if you do not need parallelism (common case) 
using namespace activelogic;

class GrazerAI{

    // GCC note: use -fno-operator-names
    status Step() task( 
        Flee(ap.threat) or Attack(ap.threat) or Graze() 
    );

    status Attack(Agent* other) task( 
        actor.Strike(other) or actor.Face(other)
    );

    // 'with' parallelizes tasks; see status_pexp.h
    status Flee(Agent* other) task(
        Evade(other, policies.safetyRange) with actor.Utter("Moo")  
    );

    status Graze() task(
        feel.hungry and Consume(ap.grass) or Roam()
    );

}
```

## Benefits and limitations

Behavior trees simplify responsive, discrete control applications (such as with tick/update functions), including networking, video games, dynamic UIs,
robotics and cognitive modeling. Tight integration enables performance with direct support for functional and object programming.

Without adjustments (or complementary techniques), stateless behavior trees are not good at handling cosmetic tasks; this isn't specific to BT. If you think of BT as a *planning tool*, similar to GOAP, tasks 
which do not cause motion (such as emoting) do not make sense.

While I find success with behavior trees, this library is only intended to get you started and encourage exploring behavior trees in a programmer 
friendly environment.

## How it works

[status](BehaviorTrees/status.h) is a three valued struct. A status may be *complete*, *running* or *failing*.

The library defines 6 operators; three operators support sequential execution

- the `and` operator is used to sequence subtasks; the second task (rh) executes if the first (lh) is complete
- the `or` operator, aka 'selector' is used to prioritize subtasks; rh executes if lh is failing
- the `over` operator is used to prioritize work (or waiting on an external task); rh executes if lh is failing or complete

Three operators support parallel execution:

- with `por` (parallel-or), rh executes if lh is not complete
- with `pand` (parallel-and), rh executes if lh is not failing
- the `with` operator is used to strictly parallelize; in this case, rh will only execute if lh is running

Often parallism is not desirable; classic behavior trees mainly use sequences and selectors. When tasks are viewed as *activities*, `over` is useful
because you need not differentiate the fail/complete status of underlying subtasks:

```
safety() over work() over rest()
```

The above says that safety matters more than getting the work done, however if safety isn't actionable, work proceeds. Likewise resting is okay, be 
work either complete, or failing.

Parallelism is useful both when approaching tasks from several angles, or the handling of multimedia applications; coordinating agents does not require special operators:

```
status Hunt(List agents, Agent* target){
    foreach(Agent* hunter in agents){
        if(hunter.Chase(target).Complete() return done;
    }
}
```

## Usage notes

a *task* is a functional block returning a status; the following is valid:

```
status Graze(){ 
    Entity* g = ap.Find("Grass");
    task( feel.hungry and Consume(g) or Roam() );
);
```

a *substask* is an expression returning status:

```
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
