#pragma once
#include "status.h"

// GCC NOTE: use -fno-operator-names

// Task
#define task(EXP) { status s; (s = EXP).complete(); return s; }

// Subtask
#define subtask(x) [=] task(x) ()

// Fallback operator; run RH if LH is failing
#define or  ).pending() || (s =

// Sequence operator; run RH if LH is done
#define and ).impending() || (s = 

// Work priority operator; run RH if LH is not running
#define over ).running() || (s =