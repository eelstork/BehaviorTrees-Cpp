#pragma once
#include "status-exp.h"

// Co-work operator. Execute RH iff LH is running
#define with ).immediate() || (s =

// Parallel selector; allows LH, RH running simultaneously,
// however if LH is succeeding, RH will not run.
#define por  ).complete() || (s =

// Parallel sequence; allows LH, RH running simultaneously,
// however if LH is failing, RH will not run.
#define pand  ).failing() || (s =
