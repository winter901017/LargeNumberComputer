#pragma once
#ifndef INFIXTOPOSTFIX_H
#define INFIXTOPOSTFIX_H

#include <iostream>
#include <vector>
#include <string>
using namespace std;

//�����ӹB��l���u���v
bool operatorPrecedenceCompare(char stackOp, char infixOp);

//�P�_�O���O�B��l
bool isOperator(char op);

//�P�_�O���O�Ʀr
bool isDigit(char c);

//�^�ǹB��l�X���|���u���v
int OutStackPriority(char op);

//�^�ǹB��l����|���u���v
int InStackPriority(char op);

//�D�禡 �N�����ন���
string infixToPostfix(string post);



#endif // !InfixToPostfix
