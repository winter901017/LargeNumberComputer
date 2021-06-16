#include "BigNumber.h"
#include "BigDecimal.h"
#include "PostfixCompute.h" 
#include <vector>
/*
Reference
https://blog.csdn.net/low5252/article/details/104871246
https://blog.csdn.net/BestZem/article/details/52744256
https://www.mdeditor.tw/pl/gQ9k/zh-tw
https://codertw.com/%E7%A8%8B%E5%BC%8F%E8%AA%9E%E8%A8%80/611857/
http://it-easy.tw/c-super-large-power/
https://blog.csdn.net/lisp1995/article/details/52347655
http://www.geocities.ws/goodprimes/FZFactorial.html
*/
BigNumber::BigNumber() //constract
{
	this->sign = true;
	this->error = 0;
};
BigNumber::~BigNumber() //deconstract
{

};

BigNumber::BigNumber(const char *s) 
{
	string str = s;
	*this = BigNumber(str);
	this->error = 0;
};

BigNumber::BigNumber(string str)
{
	/* 先不管小數 */
	//先判斷是不是數字
	//判斷是不是浮點數 
	//判斷是不是負數
	this->error = 0;
	bool isNum = true;

	/* 判斷是不是數字 */
	for (int i = 0; i < (int)str.length(); i++)
	{
		if (!(isDigit(str[i]) && (str[i] != '.'))) //不是 數字與小數點 就不是數字
		{
			isNum = false;
		}
	}

	/* 如果是正確的數字 */
	if (isNum) 
	{
		for (int i = 0; i < (int)str.length(); i++)
		{
			this ->numerator.push_back(str[i] - '0'); //轉成 一般數字 好運算
		}
		sign = true; //初始化 預設為正數
		reverse(this->numerator.begin(), this ->numerator.end()); //翻轉 987654321 變 123456789 比較好運算
	}
	else 
	{
		*this = PostfixCompute(infixToPostfix(str));
	}

};

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------
											加法
---------------------------------------------------------------------------------------------------------------------------------------------------------------
*/

void BigNumber::Add(BigNumber& b)
{
	//先找出最大的數字 (防止邊界有超出)
	//進行加法
	//一次性進位 (錯誤率較低)
	//要都是正數才可以加
	//當兩個是負 就繼續加 如果一個負 則切去減法
	//  a + -b 減法 轉換成 a - b (B要變正號)
	// -a +  b 交換 減法
	//  a +  b 正常運算
	// -a + -b 正常加法

	if (this->sign && !b.sign) //正 負
	{
		b.sign = true; // a + -b 減法 轉換成 a - b (B要變正號)
		this->Subtract(b);
	}
	else if (!this->sign && b.sign) //負 正
	{
		this->sign = true;	// -a +  b 交換 減法 轉換成 b - a (A要變正號)
		swap(*this, b);
		this->Subtract(b);
	}
	else //正常運算
	{
		if (this->numerator.size() < b.numerator.size())
		{
			swap(this->numerator, b.numerator);
		}


		for (int i = 0; i < (int)b.numerator.size(); i++)
		{
			this->numerator[i] += b.numerator[i];
			if (i != (this->numerator.size() - 1)) //當最大位數需要進位時 需特殊處理
			{
				if (this->numerator[i] >= 10) //進位
				{
					this->numerator[i + 1] += 1;
					this->numerator[i] -= 10;
				};
			}
			else
			{
				if (this->numerator[i] >= 10) //進位
				{
					this->numerator.push_back(1);
					this->numerator[i] -= 10;
				};
			};
		}

		/* 防止有0輸出 (因為相減削掉會有0 所以要清除)  */
		for (int i = this->numerator.size() - 1; i > 0; i--)
		{
			if (this->numerator[i] == 0)
			{
				this->numerator.pop_back();
			}
			else
			{
				break;
			}
		};

		/* 防止 -0 的出現 */
		if (this->numerator[0] == 0 && this->numerator.size() == 1)
		{
			this->sign = true;
		}
	};
};

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------
											減法
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
void BigNumber::Subtract(BigNumber& b) 
{
	//一樣要判斷正負號
	//要借位
	/*
	1. A > B 照做
	2. A < B 交換 補負號 (如果長度相等,則需要比較內容) 完全一樣照減 否則須補負號與交換
	*/

	//兩個都是負
	//一正一負
	//兩個都是正
	/*
	  A -  B 正常減法
	  A - -B 加法 變正 B變正號
	 -A -  B 減法 交換加法 兩個都負的
	 -A - -B 交換 減法
	*/
	if (this->sign && !b.sign)
	{
		b.sign = true;  //A - -B 加法 變正 B變正號
		this->Add(b);
	}
	else if (!this->sign && b.sign)
	{
		b.sign = false;	//-A - B 減法 交換加法 兩個都負的 轉換成 -A + -B
		swap(*this, b);

		this->Add(b);
	}
	else {

		if (compareNum(*this, b) == -1) { //B > A,減法 所以要交換
			swap(this->numerator, b.numerator); //交換vector
			this->sign = !this->sign; //相反
		}

		for (int i = 0; i < (int)b.numerator.size(); i++) {
			this->numerator[i] -= b.numerator[i];
		};
		for (int i = 0; i < (int)this->numerator.size(); i++)
		{
			if (i != (this->numerator.size() - 1)) //當最大位數需要進位時 需特殊處理
			{
				/* 當不是最大一位 */
				if (this->numerator[i] < 0) //借位
				{
					this->numerator[i + 1] -= 1;
					this->numerator[i] += 10;
				};
			}
			else
			{
				/* 最大一位的特殊處理 */
				if (this->numerator[i] < 0) ///補位
				{
					if (!this->numerator.empty()) //防止bug
					{
						this->numerator.pop_back();
					};
					this->numerator[i] += 10;
				};
			};
		};

		/* 因為相減會有0 例如 123-100 = 023 要清除 0 */
		for (int i = this->numerator.size() - 1; i > 0; i--)
		{
			if (this->numerator[i] == 0)
			{
				this->numerator.pop_back();
			}
			else
			{
				break;
			}
		};

		/* 防止 -0 的出現 */
		if (this->numerator[0] == 0 && this->numerator.size() == 1)
		{
			this->sign = true;
		}
	};
}

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------
											乘法
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
void BigNumber::Multiply(BigNumber& b) 
{
	/*
						Max: (this.size * a.size) digit
						this  2	 1	0 index
						   a  2  1	0 index

		  1 2 3				  1  2  3
		* 1 2 3			    * 1  2  3
	 -----------		--------------
		  3	6 9			  	  3  6  9
		2 4	6		->	   2  7 12      
	  1	2 3			    1  4 10
	 -----------        --------------
	  1	5 1	2 9			1  4 10	12	9
						-------------
						1  5  1	 2	9  一次性進位
					tmp 4  3  2  1  0  index

	*/
	BigNumber tmp;

	/* 預先進行最大補位 免得麻煩 (<=是為了進位)*/
	for (int i = 0; i <= (int)this->numerator.size() * (int)b.numerator.size();i++)
	{
		tmp.numerator.push_back(0); //先放入數字
	};

	/* 進行乘法 */
	for (int i = 0; i < (int)this->numerator.size(); i++)
	{
		for (int j = 0; j < (int)b.numerator.size();j++)
		{
			tmp.numerator[i + j] += this->numerator[i] * b.numerator[j];
		};
	};

	/* 進行進位 */
	for(int i = 0;i < (int)tmp.numerator.size(); i++)
	{
		/*效率太差
		if (tmp.numerator[i] >= 10)
		{
			tmp.numerator[i + 1] += tmp.numerator[i] / 10;
			tmp.numerator[i] %= 10;
		};
		*/
		while (tmp.numerator[i] >= 10)
		{
			tmp.numerator[i] -= 10;
			tmp.numerator[i + 1] ++;
			
		};
	}

	/* 正負號判斷 */
	//  A  *  A
	//  A  * -A
	// -A  *  A
	// -A  * -A
	// tmp initialize sign is positive
	if ((this->sign && !b.sign) || (!this->sign && b.sign))
	{
		tmp.sign = false; //一正 一負 = 負
	};

	/* return */
	*this = tmp;

	/* 因為相乘會有0 例如 123*0 = 000 要清除0 和 前面有先補位 防止乘法沒達到最大位數 (防止開頭有0) */

	for (int i = this->numerator.size() - 1; i > 0; i--)
	{
		if (this->numerator[i] == 0)
		{
			this->numerator.pop_back();
		}
		else
		{
			break;
		}
	};

	/* 防止 -0 的出現 */
	if (this->numerator[0] == 0 && this->numerator.size() == 1)
	{
		this->sign = true;
	}
};

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------
											除法
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
void BigNumber::Divide(BigNumber& b) 
{
	// 除法 在大數要用減的
	/*
	* 
	 this	5  5  0
	index   2  1  0 

	    a      2  4
	index      1  0 

		550
	   -24
	   -----
	    310
	   -24
	   -----
	     70
		-24
	   -----
	     46
		-24
	   -----
	     22
	（ 550 / 24 = 22 餘 22 ）

	*/
	//除法  必須A>B 否則是0   B == 0 則 不成立
	BigNumber zero("0");
	if (compareNum(*this, b) == 1 && !equalNum(b,zero) )//判斷b是不是0跟 A>B
	{
		//a為被除數，b為除數，q為商數，r為餘數。
		BigNumber tmp, quotient;
		string strTmp;

		/* 正負號判斷 */
		//  A  /  A
		//  A  / -A
		// -A  /  A
		// -A  / -A
		// quotient initialize sign is positive
		if ((this->sign && !b.sign) || (!this->sign && b.sign))
		{
			quotient.sign = false; //一正 一負 = 負
		};

		/* 因為過程會有相減 所以數字必須為正 */
		this->sign = true;
		b.sign = true;

		for (int i = this->numerator.size()-1; i >= 0; i--)
		{
			int q = 0;
			//       5 5 0 因為數字要reverse才可以計算 所以插入要放在第一個
			//index  2 1 0
			tmp.numerator.insert(tmp.numerator.begin(),this->numerator[i]); 

			/* 因為下面有做減運算 所以要消0 */
			for (int j = tmp.numerator.size() - 1; j > 0; j--)
			{
				if (tmp.numerator[j] == 0)
				{
					tmp.numerator.pop_back();
				}
				else
				{
					break;
				};
			}

			//當 減還沒有需要補位 則不須跳出
			while (compareNum(tmp, b) == 1)
			{
				tmp.Subtract(b);
				q++;
			}
			quotient.numerator.insert(quotient.numerator.begin(),q); //商數如上圖 要用插入的 (因為用除數10的被數在減)
		}

		/* 清除暫存的0*/
		for (int i = quotient.numerator.size() - 1; i > 0; i--)
		{
			if (quotient.numerator[i] == 0)
			{
				quotient.numerator.pop_back();
			}
			else
			{
				break;
			};
		};

		

		/* return */
		*this = quotient;
		
		/* 防止 -0 的出現 */
		if (this->numerator[0] == 0 && this->numerator.size() == 1)
		{
			this->sign = true;
		}
		
	}
	else/* 因為A < B 所以是0 */
	{	
		*this = BigNumber("0");
	}
}

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------
											餘除
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
void BigNumber::Remainder(BigNumber& b)
{
	// 餘除 (大數除法要用減的 剩下的數字就是餘數)
	/*
	 this	5  5  0
	index   2  1  0

		a      2  4
	index      1  0

		550
	   -24
	   -----
		310
	   -24
	   -----
		 70
		-24
	   -----
		 46
		-24
	   -----
		 22
	（ 550 / 24 = 22 餘 22 ）

	*/
	BigNumber zero("0");
	if (compareNum(*this, b) == 1 && !equalNum(b, zero))//判斷b是不是0跟 A>B
	{
		//a為被除數，b為除數，q為商數，r為餘數。
		BigNumber tmp;
		string strTmp;

		/* 餘除 不管正負*/
		/* 因為過程會有相減 所以數字必須為正 */
		this->sign = true;
		b.sign = true;

		for (int i = this->numerator.size() - 1; i >= 0; i--)
		{
			//       5 5 0 因為數字要reverse才可以計算 所以插入要放在第一個
			//index  2 1 0
			tmp.numerator.insert(tmp.numerator.begin(), this->numerator[i]);

			/* 因為下面有做減運算 所以要消0 */
			for (int j = tmp.numerator.size() - 1; j > 0; j--)
			{
				if (tmp.numerator[j] == 0)
				{
					tmp.numerator.pop_back();
				}
				else
				{
					break;
				};
			}

			//當 減還沒有需要補位 則不須跳出
			while (compareNum(tmp, b) == 1)
			{
				tmp.Subtract(b);
			}
		}

		/* 清除暫存的0*/
		for (int i = tmp.numerator.size() - 1; i > 0; i--)
		{
			if (tmp.numerator[i] == 0)
			{
				tmp.numerator.pop_back();
			}
			else
			{
				break;
			};
		};

		/* return */
		*this = tmp;

		/* 防止 -0 的出現 */
		if (!this->numerator.empty())
		{
			if (this->numerator[0] == 0 && this->numerator.size() == 1)
			{
				this->sign = true;
			};
		};
	}
	else/* 因為A < B 所以是0 */
	{
		*this = BigNumber("0");
	}
}

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------
											冪次
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
void BigNumber::Power(BigNumber& b) 
{
	/* 快速幂 */
	// 7^10 先求  7^5 在平方 如果是基數 計算n-1次方在乘上n 7^9 求 7^4的次方再乘7
	// 7^0 = 1;
	// power要是正的
	// base 奇數要判斷正負 (因為base有可能是負數) (暫時不處理)

	BigNumber zero("0"), one("1"), nOne("-1");
	//	a^0	= 1
	if (equalNum(b, zero))
	{
		*this = one;
	}
	else if (equalNum(*this, zero) && !b.sign) {
		this->error = 5;
	}
	else
	{
		BigNumber base = *this;
		//	冪次項為 + 號
		if (b.sign)
		{
			while (equalNum(b, one) == false)
			{
				this->Multiply(base);
				b.Subtract(one);
			}
		}
		//	冪次項為 - 號
		else
		{
			BigDecimal ans = BigDecimal("");
			//	有問題
			while (equalNum(b, nOne) == false)
			{
				this->Multiply(base);
				b.Add(one);
			}
			ans.BigDecimal::Divide(*this);
			*this = ans;
		};
	};

}  
	
/*-------------------------------------------------------------------------------------------------------------------------------------------------------------- -
											階層
---------------------------------------------------------------------------------------------------------------------------------------------------------------- - */
void BigNumber::Factorial() 
{
	/* N! 是指 N的階乘 (Factorial) ，即 N! = N(N-1)(N-2)......(1)，由N 乘至1，而我們亦定義 0! = 1。 */
	BigNumber zero("0"), one("1"), base("1");
	
	if (this->sign) //防止負號階層的產生
	{
		if (!equalNum(*this, zero)) // 0! = 1 特殊判定

		{	/* N! = N(N-1)(N-2)......(1) 乘到等於1 */ 
			while (!equalNum(one,*this)) { 
				base.Multiply(*this);
				this->Subtract(one);
			}
			*this = base;
		}
		else 
		{
			*this = one; // 0! = 1 特殊判定
		}
	}
	else // 當底數小於0
	{
		this->error = 4;
		//cout << "noNegative" << endl;
		//*this = zero;
	}
	
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------
											輸出
-----------------------------------------------------------------------------------------------------------------------------------------------------------------*/
string BigNumber::print()
{
	string ans;
	stringstream output;

	if (this->error == 0) {
		/* 判斷是否有負號 */
		if (!this->sign && !this->numerator.empty())
		{
			ans += '-';
		};

		/* 輸出 */
		for (int i = this->numerator.size() - 1; i >= 0; i--)
		{
			output << this->numerator[i];
		};

		/* 如果是空的表示0 運算元都消完了 */
		/*
		if (this->numerator.empty())
		{
			cout << 0;
		}
		*/
		ans += output.str();
	}
	else if (this->error == 1)
	{
		ans = "未宣告的變數";
	}
	else if (this->error == 2)
	{
		ans = "運算式錯誤，請重新檢查";
	}
	else if (this->error == 3)
	{
		ans = "0不可當做除數";
	}
	else if (this->error == 4)
	{
		ans = "不符合階層規則	(負數/小數)";
	}
	else if (this->error == 5)
	{
		ans = "不符合冪次規則	EX:[ 0^(-1) ]";
	}


	return ans;
};



/*---------------------------------------------------------------------------------------------------------------------------------------------------------------
											補數
-----------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void BigNumber::Negative()
{
	 this->sign = !this->sign;
	 if (this->numerator.size() == 1 && this->numerator[0] == 0)
		 this->sign = false;
};


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------
											比較數字大小
-----------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*-1是 A.size < B.size 要加負號 跟 交換
1 是 A.size > B.size 不做任何事情*/
 int compareNum(BigNumber a, BigNumber b) 
{
	if (a.numerator.size() > b.numerator.size())
	{
		return 1; //不做任何事情
	}
	else if (a.numerator.size() < b.numerator.size())
	{
		return -1; //要加負號 跟 交換
	}
	else if (a.numerator.size() == b.numerator.size())
	{
		for (int i = a.numerator.size()-1; i >= 0; i--)
		{
			if (a.numerator[i] > b.numerator[i]) 
			{
				return 1;//不做任何事情
			}
			else if (a.numerator[i] < b.numerator[i])
			{
				return -1; //要加負號 跟 交換
			}
		}
		return 1; //不做任何事情
	};
	return 1; //不做任何事情
};

 /*---------------------------------------------------------------------------------------------------------------------------------------------------------------
											比較數字相等
-----------------------------------------------------------------------------------------------------------------------------------------------------------------*/
//True是一樣 false是不一樣
 bool equalNum(BigNumber a, BigNumber b) 
 {
	 if (a.numerator.size() > b.numerator.size())
	 {
		 return false; //不做任何事情
	 }
	 else if (a.numerator.size() < b.numerator.size())
	 {
		 return false; //要加負號 跟 交換
	 }
	 else if (a.numerator.size() == b.numerator.size())
	 {
		 for (int i = a.numerator.size() - 1; i >= 0; i--)
		 {
			 if (a.numerator[i] > b.numerator[i])
			 {
				 return false;//不做任何事情
			 }
			 else if (a.numerator[i] < b.numerator[i])
			 {
				 return false; //要加負號 跟 交換
			 }
		 }
		 return true; //不做任何事情
	 };
	 return true; //不做任何事情
 }

 /*
 ---------------------------------------------------------------------------------------------------------------------------------------------------------------
											判斷是不是浮點數
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
 bool BigNumber::isDecimal() 
 {
	 return false;
 }

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------
										   回傳分母1
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
 vector<int> BigNumber::getDenominator()
 {
	 vector<int> tmp;
	 tmp.push_back(1);
	 return tmp;
 };

 /*
---------------------------------------------------------------------------------------------------------------------------------------------------------------
										   設置分母
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
 void BigNumber::setDenominator(vector<int> a) {

 }
/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------
										   存放變數型態(紅黑樹)
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
 map<string, BigNumber> BigNumber::bigNumbers = {};



 /*
---------------------------------------------------------------------------------------------------------------------------------------------------------------
										  >>輸入重載
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
 istream& operator>>(istream& input, BigNumber &n)
 {
	 string str;
	 input >> str;
	 if(n.isDecimal())
	 {
		 BigDecimal *tmp = new BigDecimal(str);
		 n = *tmp;
	 }
	 else
	 {
		 auto itPoint = str.find("."); //找小數點
		 if (itPoint != string::npos)
		 {
			 while (str.back() != '.')
			 {
				 str.pop_back(); //清除小數點後面的數字
			 }
			 str.pop_back(); //清除小數點
		 }
		 BigNumber* tmp = new BigNumber(str);
		 n = *tmp;
	 }


	 return input;
 }

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------
										    <<輸出重載
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
 ostream& operator<<(ostream& os,  BigNumber &n)
 {
	 if (n.isDecimal()) {
		 BigDecimal ans(n);
		 string result;
		 ans.setDenominator(n.getDenominator());
		 ans.numerator = n.numerator;
		 ans.sign = n.sign;
		 
		 result = ans.print();
		 os << result;
	 }else if (!n.isDecimal()) {
		 string result;
		 result = n.print();
		 os << result;
	 }

	 return os;
 }

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------
											加法重載
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
BigNumber& operator+(  BigNumber& A,  BigNumber& B)
{

	BigDecimal* result = new BigDecimal();
	if (!A.isDecimal() && B.isDecimal())
	{	
		BigDecimal* tmp1 = new BigDecimal(A);
		BigDecimal* tmp2 = new BigDecimal(B);
		tmp2->setDenominator(B.getDenominator());
		tmp1->Add(*tmp2);
		return *tmp1;
	}
	else if (A.isDecimal() && !B.isDecimal())
	{
		BigDecimal* tmp1 = new BigDecimal(A);
		BigDecimal* tmp2 = new BigDecimal(B);
		tmp1->setDenominator(A.getDenominator());
		tmp1->Add(*tmp2);
		return *tmp1;
	}
	else if (A.isDecimal() && B.isDecimal())
	{
		BigDecimal* tmp1 = new BigDecimal(A);
		BigDecimal* tmp2 = new BigDecimal(B);
		tmp1->setDenominator(A.getDenominator());
		tmp2->setDenominator(B.getDenominator());
		tmp1->Add(*tmp2);
		return *tmp1;
	}
	else
	{
		BigNumber* tmp1 = new BigNumber(A);
		BigNumber* tmp2 = new BigNumber(B);
		tmp1->Add(*tmp2);
		return *tmp1;
	}

	return  A;
}

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------
											減法重載
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
BigNumber& operator-( BigNumber& A,  BigNumber& B)
{
	if (!A.isDecimal() && B.isDecimal())
	{
		BigDecimal* tmp1 = new BigDecimal(A);
		BigDecimal* tmp2 = new BigDecimal(B);
		tmp2->setDenominator(B.getDenominator());
		tmp1->Subtract(*tmp2);
		return *tmp1;
	}
	else if (A.isDecimal() && !B.isDecimal())
	{
		BigDecimal* tmp1 = new BigDecimal(A);
		BigDecimal* tmp2 = new BigDecimal(B);
		tmp1->setDenominator(A.getDenominator());
		tmp1->Subtract(*tmp2);
		return *tmp1;
	}
	else if (A.isDecimal() && B.isDecimal())
	{
		BigDecimal* tmp1 = new BigDecimal(A);
		BigDecimal* tmp2 = new BigDecimal(B);
		tmp1->setDenominator(A.getDenominator());
		tmp2->setDenominator(B.getDenominator());
		tmp1->Subtract(*tmp2);
		return *tmp1;
	}
	else
	{
		BigNumber* tmp1 = new BigNumber(A);
		BigNumber* tmp2 = new BigNumber(B);
		tmp1->Subtract(*tmp2);
		return *tmp1;
	}
}

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------
											乘法重載
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
BigNumber& operator*( BigNumber& A,  BigNumber& B)
{

	if (!A.isDecimal() && B.isDecimal())
	{
		BigDecimal* tmp1 = new BigDecimal(A);
		BigDecimal* tmp2 = new BigDecimal(B);
		tmp2->setDenominator(B.getDenominator());
		tmp1->Multiply(*tmp2);
		return *tmp1;
	}
	else if (A.isDecimal() && !B.isDecimal())
	{
		BigDecimal* tmp1 = new BigDecimal(A);
		BigDecimal* tmp2 = new BigDecimal(B);
		tmp1->setDenominator(A.getDenominator());
		tmp1->Multiply(*tmp2);
		return *tmp1;
	}
	else if (A.isDecimal() && B.isDecimal())
	{
		BigDecimal* tmp1 = new BigDecimal(A);
		BigDecimal* tmp2 = new BigDecimal(B);
		tmp1->setDenominator(A.getDenominator());
		tmp2->setDenominator(B.getDenominator());
		tmp1->Multiply(*tmp2);
		return *tmp1;
	}
	else
	{
		BigNumber* tmp1 = new BigNumber(A);
		BigNumber* tmp2 = new BigNumber(B);
		tmp1->Multiply(*tmp2);
		return *tmp1;
	}

	return  A;
}

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------
											除法重載
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
BigNumber& operator/(BigNumber& A,  BigNumber& B)
{
	if (!A.isDecimal() && B.isDecimal())
	{
		BigDecimal* tmp1 = new BigDecimal(A);
		BigDecimal* tmp2 = new BigDecimal(B);
		tmp2->setDenominator(B.getDenominator());
		tmp1->Divide(*tmp2);
		return *tmp1;
	}
	else if (A.isDecimal() && !B.isDecimal())
	{
		BigDecimal* tmp1 = new BigDecimal(A);
		BigDecimal* tmp2 = new BigDecimal(B);
		tmp1->setDenominator(A.getDenominator());
		tmp1->Divide(*tmp2);
		return *tmp1;
	}
	else if (A.isDecimal() && B.isDecimal())
	{
		BigDecimal* tmp1 = new BigDecimal(A);
		BigDecimal* tmp2 = new BigDecimal(B);
		tmp1->setDenominator(A.getDenominator());
		tmp2->setDenominator(B.getDenominator());
		tmp1->Divide(*tmp2);
		return *tmp1;
	}
	else
	{
		BigNumber* tmp1 = new BigNumber(A);
		BigNumber* tmp2 = new BigNumber(B);
		tmp1->Divide(*tmp2);
		return *tmp1;
	}

	return  A;
}