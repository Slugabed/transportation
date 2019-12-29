//
// Created by master on 23.12.2019.
//
#include "test_runner.h"

void Assert(bool b, const string &hint) {
    AssertEqual(b, true, hint);
}

ostream &operator<<(ostream &os, const vector<string> &s) {
    os << "{";
    bool first = true;
    for (const auto &x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}