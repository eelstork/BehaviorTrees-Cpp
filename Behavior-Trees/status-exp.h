#pragma once

// Task preamble
#define task { status s; (s =

// Task epilogue
#define end ).complete(); return s; }

// Subtask
#define subtask(x) [=] { task x end }()

// BT selector/fallback operation
#define or  ).pending() || (s =

// BT sequence operation
#define and ).impending() || (s = 

// Work priority operator. Execute RH if LH is not running
#define over ).running() || (s =