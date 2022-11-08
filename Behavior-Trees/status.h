#pragma once
#include <iostream>

struct status
{
	int value = 0;

	status();

	status(int);

	bool complete();
	bool running();
	bool failing();
	bool impending();
	bool immediate();
	bool pending();

	//explicit operator bool() const;
	status operator !() const;
	bool operator ==(const status& y);
	bool operator !=(const status& y);

};

// Console output

std::ostream& operator<<(std::ostream& os, status const& s);

// Status constants

static status fail = status(-1);
static status cont = status(0);
static status done = status(1);

// REMOVE???

//bool is_status_pending(status arg);
//bool is_status_impending(status arg);
