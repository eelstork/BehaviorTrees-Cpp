#pragma once
#include <iostream>

namespace activelogic {
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

		status operator !() const;
		bool operator ==(const status& y);
		bool operator !=(const status& y);

	};

	// Status constants

	static status fail = status(-1);
	static status cont = status(0);
	static status done = status(1);

	// Console output

	std::ostream& operator<<(std::ostream& os, status const& s);

}


