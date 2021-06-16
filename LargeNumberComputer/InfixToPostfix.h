#pragma once
#ifndef INFIXTOPOSTFIX_H
#define INFIXTOPOSTFIX_H

#include <iostream>
#include <vector>
#include <string>
using namespace std;

//比較兩個運算子的優先權
bool operatorPrecedenceCompare(char stackOp, char infixOp);

//判斷是不是運算子
bool isOperator(char op);

//判斷是不是數字
bool isDigit(char c);

//回傳運算子出堆疊的優先權
int OutStackPriority(char op);

//回傳運算子近堆疊的優先權
int InStackPriority(char op);

//主函式 將中序轉成後序
string infixToPostfix(string post);



#endif // !InfixToPostfix
