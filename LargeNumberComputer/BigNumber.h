#pragma once

#ifndef BIGNUMBER_H
#define	BIGNUMBER_H

#include "InfixToPostfix.h" //使用到 isDigts
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
	對於父類的方法，你預期它的執行時期行為會被重新定義，也就是希望在執行時期，
	依照實例的型態綁定對應的方法版本，可以在父類定義方法時加上 virtual 

	如果直接呼叫函式 會呼叫到BigNumber的, 所以要使用 virtual (呼叫的是各自子類別的方法) 
	且需要再主程式中, 以指標的方式呼叫. 才可以正確使用 

	使用時機：在子類不覆寫 override 時，有個預設實作(父類的實作)。 
	*/

	/*!!!!!	先正常運算 在定義除錯	!!!!!!*/
public:

	bool sign = true; // 正負號 (true 是正 false是負 )
	vector <int> numerator; // 分子  ( 要用分數的方式處理 後來才可以處理浮點數 分母是1 所以省略 )
	int error = 0; // 錯誤代碼

	BigNumber(); // 空建構子
	~BigNumber(); // deconstract
	BigNumber(string str); // 建構子(防呆應該要寫在裡面)
	BigNumber(const char* s);
	virtual void Add(BigNumber& b); // 加法
	virtual void Subtract(BigNumber& b);// 減法
	virtual void Multiply(BigNumber& b); // 乘法
	virtual void Divide(BigNumber& b); // 除法
	void Remainder(BigNumber& b); // 只會用在整數 不須重新建構函式 所以不加virtual
	virtual void Power(BigNumber& b); // 冪次
	virtual void Factorial(); // 階層
	virtual void Negative(); // 進行補數 使用在負號變正號 正號變負號
	virtual string print(); // 輸出 numerator
	virtual bool isDecimal(); // 在PostfixCompute 判斷是不是浮點數 (根據class判斷)
	virtual vector<int> getDenominator(); //得到分母 1
	virtual void setDenominator(vector<int> a);
	friend bool equalNum(BigNumber a, BigNumber b); // True是一樣 false是不一樣
	friend int compareNum(BigNumber a, BigNumber b); // -1是 A<B, 0 是 A=B , 1 是 A>B

	friend BigNumber& operator+( BigNumber& A,  BigNumber& B); //加法重載
	friend BigNumber& operator-( BigNumber& A,  BigNumber& B); //減法重載
	friend BigNumber& operator*( BigNumber& A,  BigNumber& B); //加法重載
	friend BigNumber& operator/( BigNumber& A,  BigNumber& B); //減法重載

	friend istream& operator>>(istream&, BigNumber&); //輸入重載
	friend ostream& operator<<(ostream&,  BigNumber&); //輸出重載

	static map <string, BigNumber> bigNumbers; // 拿來存放 Set Integer A = 0 存放變數名稱A與數值
};

#endif  // ! BIGNUMBER_H

