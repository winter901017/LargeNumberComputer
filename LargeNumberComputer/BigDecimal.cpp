#include "BigDecimal.h"
#include "PostfixCompute.h" 

BigDecimal::BigDecimal() 
{
	this->sign = true;
}

BigDecimal::~BigDecimal()
{

}

BigDecimal::BigDecimal(const char* s) {
	string str = s;
	*this = BigDecimal(str);
}

BigDecimal::BigDecimal(BigNumber copy)
{
	this->sign = copy.sign;
	this->numerator = copy.numerator; 
	this->denominator.push_back(1);
};

BigDecimal::BigDecimal(string str)
{
	//先判斷是不是數字
	//判斷是不是浮點數 
	//判斷是不是負數
	bool isNum = true;

	/* 判斷是不是數字 */
	for (int i = 0; i < (int)str.length(); i++)
	{
		if (!(isDigit(str[i]) || str[i] == '.')) //不是 數字與小數點 就不是數字
		{
			isNum = false;
		}
	}

	/* 如果是正確的數字 */
	if (isNum )
	{
		/* 判斷有沒有小數點 */
		// 有的話有補0
		// 沒有小數點則要他補點然後補零
		// 小數點後100位
		// 用分數方式實作

		bool point = true;
		int pointCount = 0;
		/* 處理分子與分母 */
		reverse(str.begin(), str.end()); //翻轉 987654321 變 123456789 比較好運算
		for (int i = 0; i < (int)str.length(); i++)
		{
			/* 防止將小數點轉換 */
			if (str[i] == '.')
			{
				point = false;
				this->denominator.push_back(0);
				this->denominator[i] = 1; /* 開始算分母 */
			}
			else
			{
				if (point)
				{
					this->denominator.insert(this->denominator.begin(),0); /* 分母補0 */
					this->numerator.push_back(str[i] - '0'); //轉成 一般數字 好運算 
					pointCount++;
				}
				else
				{
					this->numerator.push_back(str[i] - '0'); //轉成 一般數字 好運算
				};
			};

		}
		if (point)
		{
			this->denominator.insert(this->denominator.begin(), 1); // 分母補0 
			/*
			for (int i = 0; i < pointCount-1; i++) 
			{
				this->denominator.insert(this->denominator.begin(), 0); // 分母補0 
			};
			*/
		};

		/* 清除暫存的0 */
		for (int i = this->numerator.size() - 1; i > 0; i--)
		{
			if (this->numerator[i] == 0)
			{
				this->numerator.pop_back();
			}
			else
			{
				break;
			};
		};
		for (int i = this->denominator.size() - 1; i > 0; i--)
		{
			if (this->denominator[i] == 0)
			{
				this->denominator.pop_back();
			}
			else
			{
				break;
			};
		};

		/* 防止 -0 的出現 */
		if (this->numerator[0] == 0 && this->numerator.size() == 1)
		{
			this->sign = true;
		}

		sign = true; //初始化 預設為正數

	}
	else
	{
		*this = PostfixCompute(infixToPostfix(str));
		this->setDenominator(PostfixCompute(infixToPostfix(str)).getDenominator());
	}
}


/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------
											判斷是不是浮點數
---------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
bool BigDecimal::isDecimal()
{
	return true;
}

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------
											加法
---------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
void BigDecimal::Add(BigNumber& b)
{
	BigDecimal tmp(b); //如果是跟整數做運算 要轉換乘浮點數
	tmp.denominator = b.getDenominator(); //會回傳 1 
	RTCD(*this,tmp); //通分 分母相同
	BigNumber a1, a2;
	a1.numerator = this->numerator;
	a1.sign = this->sign;
	a2.numerator = tmp.numerator;
	a2.sign = tmp.sign;
	a1.Add(a2); //分子 相加
	tmp = BigDecimal(a1);
	tmp.numerator = a1.numerator;
	tmp.denominator = this->denominator; //分母傳值 (保險)
	*this = tmp;
}

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------
											減法
---------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
void BigDecimal::Subtract(BigNumber& b)
{
	BigDecimal tmp(b); //如果是跟整數做運算 要轉換乘浮點數
	tmp.denominator = b.getDenominator(); //會回傳 1 
	RTCD(*this, tmp); //通分 分母相同
	BigNumber a1, a2;
	a1.numerator = this->numerator;
	a1.sign = this->sign;
	a2.numerator = tmp.numerator;
	a2.sign = tmp.sign;
	a1.Subtract(a2); //分子 相加
	tmp = BigDecimal(a1);
	tmp.numerator = a1.numerator;
	tmp.denominator = this->denominator; //分母傳值 (保險)
	*this = tmp;
}

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------
											乘法
---------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
void BigDecimal::Multiply(BigNumber& b)
{
	BigDecimal tmp(b),ans; //如果是跟整數做運算 要轉換乘浮點數
	tmp.denominator = b.getDenominator(); //會回傳 1 
	
	BigNumber a1, a2;
	a1.numerator = this->numerator;
	a1.sign = this->sign;

	a2.numerator = tmp.numerator;
	a2.sign = tmp.sign;

	a1.Multiply(a2); //分子 相乘
	ans.sign = a1.sign;
	ans.numerator = a1.numerator;
	
	a1.numerator = this->denominator;
	a1.sign = this->sign;
	a2.numerator = tmp.denominator;
	a2.sign = tmp.sign;
	a1.Multiply(a2); //分母 相乘

	ans.denominator = a1.numerator;

	*this = ans;
}

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------
											除法
---------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
void BigDecimal::Divide(BigNumber& b)
{	
	/* 將除數倒數乘以 被除數*/
	
	BigDecimal tmp(b);
	tmp.denominator = tmp.numerator;
	tmp.numerator = b.getDenominator();
	this->Multiply(tmp);
}

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------
											平方
---------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
BigDecimal  BigDecimal::Sqrts(BigNumber b)
{
	BigNumber ans;
		return ans;
}

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------
											冪次
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
void BigDecimal::Power(BigNumber& b)
{
	
	/* 快速幂 */
	// 7^10 先求  7^5 在平方 如果是基數 計算n-1次方在乘上n 7^9 求 7^4的次方再乘7
	// 7^0 = 1;
	// power要是正的
	// base 奇數要判斷正負 (因為base有可能是負數) (暫時不處理)

	BigNumber zero("0"), one("1"), nOne("-1"),two("2");
	//	a^0	= 1
	BigNumber up;
	up.numerator = b.numerator;
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
		if (up.sign)
		{
				up.numerator[0]/=10;
			
				while (equalNum(up, zero) == false)
				{
					this->Multiply(base);
					up.Subtract(one);
				}

				if (up.numerator[0] == 5)
				{
					
				}

		}
		//	冪次項為 - 號
		else
		{
			BigDecimal ans = BigDecimal("");
			//	有問題
			while (equalNum(up, nOne) == false)
			{
				this->Multiply(base);
				up.Add(one);
			}
			ans.BigDecimal::Divide(*this);
			this->Sqrts(two);
			*this = ans;
		};
	};

	this->ReductionFraction();
	
};


/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------
											輸出
---------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
string BigDecimal::print()
{
	
	string ans;
	if (this->numerator.size() == 1 && this->numerator[0] == 0)
	{
		return "0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
	}

	this->ReductionFraction(); //通分

	stringstream output;


	/* 判斷是否有負號 */
	if (!this->sign && !this->numerator.empty())
	{
		ans += '-';
	};

	BigNumber up, down;
	up.numerator = this->numerator;
	down.numerator = this->denominator;
	BigNumber quotient, tmp;

	/*計算商數*/
	for (int i = (int)up.numerator.size() - 1; i >= 0; i--)
	{
		int q = 0;
		//       5 5 0 因為數字要reverse才可以計算 所以插入要放在第一個
		//index  2 1 0
		tmp.numerator.insert(tmp.numerator.begin(), up.numerator[i]);

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
		while (compareNum(tmp, down) == 1)
		{
			tmp.Subtract(down);
			q++;
		}
		quotient.numerator.insert(quotient.numerator.begin(), q); //商數如上圖 要用插入的 (因為用除數10的被數在減)
	}

	/* 清除暫存;商數的0 */
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

	/* 計算餘數 商數後面接著算 直到餘數有一百位 */
	BigNumber remainder;
	for (int i = 0;  remainder.numerator.size() < 100; i++) 
	{
		int r = 0;
		//       5 5 0 因為數字要reverse才可以計算 所以插入要放在第一個
		//index  2 1 0
		tmp.numerator.insert(tmp.numerator.begin(), 0); //會一直補0

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

		//當 減還沒有需要補位 則不須跳出 會一直減
		while (compareNum(tmp, down) == 1)
		{
			tmp.Subtract(down);
			r++;
		}
		remainder.numerator.insert(remainder.numerator.begin(), r); //插入餘數
	};

	/* 輸出商數 */
	for (int i = quotient.numerator.size() - 1; i >= 0; i--) {
		output << quotient.numerator[i];
	}
	output << '.';
	/* 輸出餘數 */
	for (int i = remainder.numerator.size() - 1; i >= 0; i--) {
		output << remainder.numerator[i];
	}
	ans += output.str();
	output.str("");
	return ans;
}


/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------
											通分
---------------------------------------------------------------------------------------------------------------------------------------------------------------
*/

void RTCD(BigDecimal& a, BigDecimal& b) 
{
	/*
	  2      3
	---- + ---- 先找 分母的 GCD 再將分母直接相乘 除上 GCD 就是LCM 兩數把LCM除上分母乘上分子
	 30     16
	*/ 

	BigDecimal aUp, aDown;
	BigDecimal bUp, bDown;
	aUp.numerator = a.numerator;
	bUp.numerator = b.numerator;
	aDown.numerator = a.getDenominator();
	bDown.numerator = b.getDenominator();
	BigNumber zero("0"), GCD, LCM;
	/* 輾轉相除法 */
	while (!equalNum(aDown,zero)) 
	{ 
		if (compareNum(aDown, bDown) == -1)
		{
			swap(aDown, bDown);
		}
		else
		{
			aDown.Remainder(bDown);
		}
	}
	GCD = bDown; //最大公因數

	aDown.numerator = a.getDenominator(); // 重新給值
	bDown.numerator = b.getDenominator(); // 重新給值

	aDown.Multiply(bDown);
	aDown.Divide(GCD);
	LCM = aDown; //最大公因數 最小公倍數
	BigNumber tmpA,tmpB;
	tmpA = LCM;
	tmpB = LCM;
	aDown.numerator = a.getDenominator(); // 重新給值
	bDown.numerator = b.getDenominator(); // 重新給值

	tmpA.Divide(aDown);
	tmpB.Divide(bDown);
	a.RTCD_Multiply(tmpA);
	b.RTCD_Multiply(tmpB);

};

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------
											通分要用的乘法
---------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
void BigDecimal::RTCD_Multiply(BigNumber& b)
{
	BigDecimal tmp;

	/* 預先進行最大補位 免得麻煩 (<=是為了進位)*/
	for (int i = 0; i <= (int)this->numerator.size() * (int)b.numerator.size(); i++)
	{
		tmp.numerator.push_back(0); //先放入數字
	};
	for (int i = 0; i <= (int)this->denominator.size() * (int)b.numerator.size(); i++)
	{
		tmp.denominator.push_back(0); //先放入數字
	};

	/* 進行乘法 */
	for (int i = 0; i < (int)this->numerator.size(); i++)
	{
		for (int j = 0; j < (int)b.numerator.size(); j++)
		{
			tmp.numerator[i + j] += this->numerator[i] * b.numerator[j];
		};
	};
	for (int i = 0; i < (int)this->denominator.size(); i++)
	{
		for (int j = 0; j < (int)b.numerator.size(); j++)
		{
			tmp.denominator[i + j] += this->denominator[i] * b.numerator[j];
		};
	};

	/* 進行進位 */
	for (int i = 0; i < (int)tmp.numerator.size(); i++)
	{
		while (tmp.numerator[i] >= 10)
		{
			tmp.numerator[i] -= 10;
			tmp.numerator[i + 1] ++;

		};
	}
	for (int i = 0; i < (int)tmp.denominator.size(); i++)
	{
		while (tmp.denominator[i] >= 10)
		{
			tmp.denominator[i] -= 10;
			tmp.denominator[i + 1] ++;

		};
	}
	/* 正負號 return */
	tmp.sign = this->sign; 

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
	for (int i = this->denominator.size() - 1; i > 0; i--)
	{
		if (this->denominator[i] == 0)
		{
			this->denominator.pop_back();
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
	if (this->denominator[0] == 0 && this->denominator.size() == 1)
	{
		this->sign = true;
	}
};

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------
											約分
---------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
void BigDecimal::ReductionFraction() 
{
	BigDecimal aUp, aDown;
	aUp.numerator = this->numerator;
	aDown.numerator = this->getDenominator();
	BigNumber zero("0"), GCD, LCM;
	/* 輾轉相除法 */
	while (!equalNum(aDown, zero))
	{
		if (compareNum(aUp, aDown) == -1)
		{
			swap(aUp, aDown);
		}
		else
		{
			aUp.Remainder(aDown);
		}
	}
	GCD = aDown; //最大公因數

	aUp.numerator = this->numerator; // 重新給值
	aDown.numerator = this->getDenominator(); // 重新給值

	aUp.Divide(GCD);
	aDown.Divide(GCD);
	this->numerator = aUp.numerator;
	this->denominator = aDown.numerator;

}
/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------
											取得分母
---------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
vector <int>  BigDecimal::getDenominator() 
{
	return this->denominator;
}

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------
											修改分母
---------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
void BigDecimal::setDenominator(vector <int> copy)
{
	this->denominator = copy;
};


/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------
										   存放變數型態(紅黑樹)
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
map<string, BigDecimal> BigDecimal::bigDecimals = {};

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------
										  >>輸入重載
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
istream& operator>>(istream& input, BigDecimal& n)
{
	string str;
	input >> str;
	BigDecimal tmp(str);
	n = tmp;

	return input;
}

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------
											<<輸出重載
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
ostream& operator<<(ostream& os, const BigDecimal& n)
{
	if (!n.sign)
	{
		os << '-';
	};

	for (int i = (int)n.numerator.size() - 1; i >= 0; i--)
	{
		os << n.numerator[i];
	}
	os << '/';
	for (int i = (int)n.denominator.size() - 1; i >= 0; i--)
	{
		os << n.denominator[i];
	}
	return os;
}

