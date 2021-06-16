#pragma once
#ifndef BIGDECIMAL_H
#define	BIGDECIMAL_H

#include "BigNumber.h"
/*
* 
Reference
https://ithelp.ithome.com.tw/articles/10230444

*/


/* �n���~�Ӫ��覡 �W�Ҧ�����(���O��?)*/

/* Inherit BigNumber Public */
class BigDecimal:
	public BigNumber // �~��BigNumber��public
{
private:

	vector<int> denominator; // �]���Τp�ƹB�� �ҥH�n����

public:

	BigDecimal(); //�ūغc�l
	~BigDecimal(); //deconstract
	BigDecimal(string str); //�غc�l(���b���ӭn�g�b�̭�)
	BigDecimal(BigNumber copy); // �ƻsclass
	BigDecimal(const char* s);
	bool isDecimal(); // �P�_�O���O�B�I�� (�ھ�class��ƫ��A�ӧP�_) 

	void Add(BigNumber& b); // �[�k
	void Subtract(BigNumber& b); // ��k
	void Multiply(BigNumber& b); // ��
	void Divide(BigNumber& b); // ��
	void Power(BigNumber& b);; // ����
	BigDecimal Sqrts(BigNumber b);
	string print(); // ��X
	void ReductionFraction();
	friend void RTCD(BigDecimal& a, BigDecimal& b); // �q��
	void RTCD_Multiply(BigNumber& b); // �q���n�Ϊ����k
	vector <int> getDenominator(); //���o����
	void setDenominator(vector <int>); //�]�m����


	friend istream& operator>>(istream&, BigDecimal&); //��J����
	friend ostream& operator<<(ostream&, const BigDecimal&); //��X����

	static map <string, BigDecimal> bigDecimals; // ���Ӧs�� Set Integer A = 0 �s���ܼƦW��A�P�ƭ�
	/* �H�U�O�~�Ӫ��F��

	bool sign = true; //���t�� (true �O�� false�O�t )
	vector <int> numerator; //���l  ( �n�Τ��ƪ��覡�B�z ��Ӥ~�i�H�B�z�B�I�� �����O1 �ҥH�ٲ� )
	int error = 0; //���~�N�X


	BigNumber(); //�ūغc�l
	~BigNumber(); //deconstract
	BigNumber(string str); //�غc�l(���b���ӭn�g�b�̭�)
	void Add(BigNumber& b); // �[�k
	void Subtract(BigNumber& b);//��k
	void Multiply(BigNumber& b); //���k
	void Divide(BigNumber& b); //���k
	void Power(BigNumber& b); //����
	void Factorial(); //���h
	void Nagetive(); //�i��ɼ� �ϥΦb�t���ܥ��� �����ܭt��
	void print(); //��X numerator
	
	friend bool equalNum(BigNumber a, BigNumber b); //True�O�@�� false�O���@��
	friend int compareNum(BigNumber a, BigNumber b); //-1�O A<B, 0 �O A=B , 1 �O A>B

	*/
};


#endif  // ! BigDecimal_H

