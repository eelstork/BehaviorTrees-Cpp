#include "pch.h"
#include "status.h"
#include <iostream>

status::status() {}

status::status(int x) { value = x; }

bool status::complete() { return value == 1; }

bool status::failing() { return value == -1; }

bool status::immediate() { return value != 0; }

bool status::running() { return value == 0; }

bool status::pending() { return value != -1; }

bool status::impending() { return value != +1; };

status status::operator !() const { return status(-value); }

bool status::operator ==(const status& y) {
    return value == y.value;
}

bool status::operator !=(const status& y) {
    return value != y.value;
}

std::ostream& operator<<(std::ostream& os, status const& s) {

    switch (s.value) {
    case -1:
        return os << "fail";
    case  0:
        return os << "cont";
    case  1:
        return os << "done";
    default:
        return os << "?status(" << s.value << ")";
    }

}