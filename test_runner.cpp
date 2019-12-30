//
// Created by master on 23.12.2019.
//
#include "test_runner.h"

void Assert(bool b, const string &hint) {
    AssertEqual(b, true, hint);
}