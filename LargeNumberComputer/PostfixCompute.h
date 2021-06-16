#pragma once
#ifndef POSTFIXCOMPUTE_H
#define POSTFIXCOMPUTE_H
#include "BigNumber.h"
#include <string>
#include <vector>
using namespace std;

//進行後序的計算
BigNumber& PostfixCompute(string postfix);

#endif // !POSTFIXCOMPUTE_H