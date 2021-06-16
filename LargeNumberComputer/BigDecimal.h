#pragma once
#ifndef BIGDECIMAL_H
#define	BIGDECIMAL_H

#include "BigNumber.h"
/*
* 
Reference
https://ithelp.ithome.com.tw/articles/10230444

*/


/* 要用繼承的方式 上課有說到(像是樹狀?)*/

/* Inherit BigNumber Public */
class BigDecimal:
	public BigNumber // 繼承BigNumber的public
{
private:

	vector<int> denominator; // 因為用小數運算 所以要分母

public:

	BigDecimal(); //空建構子
	~BigDecimal(); //deconstract
	BigDecimal(string str); //建構子(防呆應該要寫在裡面)
	BigDecimal(BigNumber copy); // 複製class
	BigDecimal(const char* s);
	bool isDecimal(); // 判斷是不是浮點數 (根據class資料型態來判斷) 

	void Add(BigNumber& b); // 加法
	void Subtract(BigNumber& b); // 減法
	void Multiply(BigNumber& b); // 乘
	void Divide(BigNumber& b); // 除
	void Power(BigNumber& b);; // 冪次
	BigDecimal Sqrts(BigNumber b);
	string print(); // 輸出
	void ReductionFraction();
	friend void RTCD(BigDecimal& a, BigDecimal& b); // 通分
	void RTCD_Multiply(BigNumber& b); // 通分要用的乘法
	vector <int> getDenominator(); //取得分母
	void setDenominator(vector <int>); //設置分母


	friend istream& operator>>(istream&, BigDecimal&); //輸入重載
	friend ostream& operator<<(ostream&, const BigDecimal&); //輸出重載

	static map <string, BigDecimal> bigDecimals; // 拿來存放 Set Integer A = 0 存放變數名稱A與數值
	/* 以下是繼承的東西

	bool sign = true; //正負號 (true 是正 false是負 )
	vector <int> numerator; //分子  ( 要用分數的方式處理 後來才可以處理浮點數 分母是1 所以省略 )
	int error = 0; //錯誤代碼


	BigNumber(); //空建構子
	~BigNumber(); //deconstract
	BigNumber(string str); //建構子(防呆應該要寫在裡面)
	void Add(BigNumber& b); // 加法
	void Subtract(BigNumber& b);//減法
	void Multiply(BigNumber& b); //乘法
	void Divide(BigNumber& b); //除法
	void Power(BigNumber& b); //冪次
	void Factorial(); //階層
	void Nagetive(); //進行補數 使用在負號變正號 正號變負號
	void print(); //輸出 numerator
	
	friend bool equalNum(BigNumber a, BigNumber b); //True是一樣 false是不一樣
	friend int compareNum(BigNumber a, BigNumber b); //-1是 A<B, 0 是 A=B , 1 是 A>B

	*/
};


#endif  // ! BigDecimal_H

