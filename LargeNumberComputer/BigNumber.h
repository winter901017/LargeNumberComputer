#pragma once

#ifndef BIGNUMBER_H
#define	BIGNUMBER_H

#include "InfixToPostfix.h" //�ϥΨ� isDigts
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <map>
using namespace std;


class BigNumber
{
	/* 
	����������k�A�A�w����������ɴ��欰�|�Q���s�w�q�A�]�N�O�Ʊ�b����ɴ��A
	�̷ӹ�Ҫ����A�j�w��������k�����A�i�H�b�����w�q��k�ɥ[�W virtual 

	�p�G�����I�s�禡 �|�I�s��BigNumber��, �ҥH�n�ϥ� virtual (�I�s���O�U�ۤl���O����k) 
	�B�ݭn�A�D�{����, �H���Ъ��覡�I�s. �~�i�H���T�ϥ� 

	�ϥήɾ��G�b�l�����мg override �ɡA���ӹw�]��@(��������@)�C 
	*/

	/*!!!!!	�����`�B�� �b�w�q����	!!!!!!*/
public:

	bool sign = true; // ���t�� (true �O�� false�O�t )
	vector <int> numerator; // ���l  ( �n�Τ��ƪ��覡�B�z ��Ӥ~�i�H�B�z�B�I�� �����O1 �ҥH�ٲ� )
	int error = 0; // ���~�N�X

	BigNumber(); // �ūغc�l
	~BigNumber(); // deconstract
	BigNumber(string str); // �غc�l(���b���ӭn�g�b�̭�)
	BigNumber(const char* s);
	virtual void Add(BigNumber& b); // �[�k
	virtual void Subtract(BigNumber& b);// ��k
	virtual void Multiply(BigNumber& b); // ���k
	virtual void Divide(BigNumber& b); // ���k
	void Remainder(BigNumber& b); // �u�|�Φb��� �������s�غc�禡 �ҥH���[virtual
	virtual void Power(BigNumber& b); // ����
	virtual void Factorial(); // ���h
	virtual void Negative(); // �i��ɼ� �ϥΦb�t���ܥ��� �����ܭt��
	virtual string print(); // ��X numerator
	virtual bool isDecimal(); // �bPostfixCompute �P�_�O���O�B�I�� (�ھ�class�P�_)
	virtual vector<int> getDenominator(); //�o����� 1
	virtual void setDenominator(vector<int> a);
	friend bool equalNum(BigNumber a, BigNumber b); // True�O�@�� false�O���@��
	friend int compareNum(BigNumber a, BigNumber b); // -1�O A<B, 0 �O A=B , 1 �O A>B

	friend BigNumber& operator+( BigNumber& A,  BigNumber& B); //�[�k����
	friend BigNumber& operator-( BigNumber& A,  BigNumber& B); //��k����
	friend BigNumber& operator*( BigNumber& A,  BigNumber& B); //�[�k����
	friend BigNumber& operator/( BigNumber& A,  BigNumber& B); //��k����

	friend istream& operator>>(istream&, BigNumber&); //��J����
	friend ostream& operator<<(ostream&,  BigNumber&); //��X����

	static map <string, BigNumber> bigNumbers; // ���Ӧs�� Set Integer A = 0 �s���ܼƦW��A�P�ƭ�
};

#endif  // ! BIGNUMBER_H

