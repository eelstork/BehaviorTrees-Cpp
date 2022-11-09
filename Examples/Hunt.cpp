#include "status-pexp.h"

using namespace activelogic;

class Agent {

public:
    status Chase(void* target) { return fail;  }

};

status Hunt(Agent hunters[], int count, Agent* target) {
    for (int i = 0; i < count; i++) {
        if (hunters[i].Chase(target).complete()) {
            return done;
        }
    }
    return cont;
}
