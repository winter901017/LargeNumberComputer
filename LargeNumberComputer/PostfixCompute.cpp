#include "PostfixCompute.h"
#include "InfixToPostfix.h"
#include "BigNumber.h"
#include "BigDecimal.h"
#include <vector>
BigNumber& PostfixCompute(string postfix) {
	/*
	想法:
	一個 ans 放 全部資料 
	一個 NumRegister 放 暫時運算元
	跑回圈 先判斷是不是運算元 否則丟到NumRegister裡當遇到空格 就分割
	NumRegister清空
	假如下個一樣是 運算元 則一樣
	如果遇到運算子 看是要抓一個數字或是兩個數字來做運算
	*/
	string ans;
	vector<BigNumber*> stack; //放入大數的運算元  /* 因為virtual 與 繼承 所以要用pointer */
	string NumRegister; //數字暫存器

	//int error = 0; 
	// error 1 size小於2 算式錯誤


	for (int index = 0; index < (int)postfix.length(); index++)
	{
		if (postfix[index] == ' ') // 表示有運算元或運算子
		{
			if (NumRegister != "")
			{
				/* 判斷是否是變數 要把她轉成表達式*/
				if ((NumRegister[0] >= 'a' && NumRegister[0] <= 'z') || (NumRegister[0] >= 'A' && NumRegister[0] <= 'Z'))
				{
					auto itInteger = BigNumber::bigNumbers.find(NumRegister);
					auto itDecimal = BigDecimal::bigDecimals.find(NumRegister);
					bool isFound = false;
					if (itDecimal != BigDecimal::bigDecimals.end())
					{
						auto expression = new BigDecimal(BigDecimal::bigDecimals[NumRegister]);
						
						stack.push_back(expression);
						isFound = true;
					}
					else if (isFound == false && itInteger != BigNumber::bigNumbers.end())
					{
						auto expression = new BigNumber(BigNumber::bigNumbers[NumRegister]);
						
						stack.push_back(expression);
					}
					else {
						//	出現未宣告變數
						BigNumber* error = new BigNumber();
						error->error = 1;
						return *error;
					}
				}
				else {

					/* 要判斷是整數還是浮點數 */
					// 小數點不只一個是錯
					int pointCount = 0;
					for (int i = 0; i < (int)NumRegister.size(); i++)
					{
						if (NumRegister[i] == '.') // 找小數點
						{
							pointCount++;
						};
					}
					if (pointCount == 0) // 沒有小數點
					{
						BigNumber* push = new BigNumber(NumRegister);
						stack.push_back(push);
					}
					else if (pointCount == 1) // 有一個小數點
					{
						BigNumber* push = new BigDecimal(NumRegister);
						stack.push_back(push);
					}
					else { // 不只一個小數點 error
						/* 因為錯誤輸入 要改成return別的東西 否則會 vector range*/
						BigNumber* error = new BigNumber();
						error->error = 2;
						return *error;
					}
				};
				NumRegister = ""; //清空暫存運算元
			}
			continue;
		}

		/* 以下是運算子的計算 */
		if (isOperator(postfix[index]) || postfix[index] == '#') 
		{
			if (postfix[index] == '#')
			{
				if (stack.size() < 1)
				{
					BigNumber* error = new BigNumber();
					error->error = 2;
					return *error;
				}
				else
				{
					stack[stack.size() - 1]->Negative();
				}

			};

			if (postfix[index] == '+')
			{
				//要判斷 正負數 與 是不是浮點數 在做運算 用operator overloading做運算
				if (stack.size() < 2) 
				{
					BigNumber* error = new BigNumber();
					error->error = 2;
					return *error;
				}
				else 
				{
					/* 當A不是小數 B是 則要swap 因為Class型態 */
					if (!stack[stack.size() - 2]->isDecimal() && stack[stack.size() - 1]->isDecimal())
					{
						swap(stack[stack.size() - 2], stack[stack.size() - 1]);
					}

					stack[stack.size() - 2]->Add(*stack[stack.size() - 1]);
					stack.pop_back();
					
				}
			}
			else if (postfix[index] == '-')
			{
				//要判斷 正負數 與 是不是浮點數 在做運算 用operator overloading做運算
				if (stack.size() < 2) 
				{
					BigNumber* error = new BigNumber();
					error->error = 2;
					return *error;
				}
				else {

					/* 當A不是小數 B是 則要swap 因為Class型態 */
					if (!stack[stack.size() - 2]->isDecimal() && stack[stack.size() - 1]->isDecimal())
					{
						swap(stack[stack.size() - 2], stack[stack.size() - 1]);
					}

					stack[stack.size() - 2]->Subtract(*stack[stack.size() - 1]);
					stack.pop_back();
				}
			}
			else if (postfix[index] == '*')
			{
				if (stack.size() < 2)
				{
					BigNumber* error = new BigNumber();
					error->error = 2;
					return *error;
				}
				else {
					/* 當A不是小數 B是 則要swap 因為Class型態 */
					if (!stack[stack.size() - 2]->isDecimal() && stack[stack.size() - 1]->isDecimal())
					{
						swap(stack[stack.size() - 2], stack[stack.size() - 1]);
					}

					stack[stack.size() - 2]->Multiply(*stack[stack.size() - 1]);
					stack.pop_back();
				}
			};

			if (postfix[index] == '/')
			{
				//	divide 
				if (stack.size() < 2)
				{
					BigNumber* error = new BigNumber();
					error->error = 2;
					return *error;
				}
				// divide 0 error
				else if (stack[stack.size() - 1]->numerator.size() == 1 && stack[stack.size() - 1]->numerator[0] == 0)
				{
					BigNumber* error = new BigNumber();
					error->error = 3;
					return *error;
				}
				else {
					/* 當A不是小數 B是 則要swap 因為Class型態 */
					if (!stack[stack.size() - 2]->isDecimal() && stack[stack.size() - 1]->isDecimal())
					{
						BigDecimal buffer;
						buffer.numerator = stack[stack.size()-1]->numerator;
						buffer.sign = stack[stack.size() - 1]->sign;
						buffer.setDenominator(stack[stack.size() - 1]->getDenominator()); //Buferrr 是 B 直接拿buffer來除
						swap(stack[stack.size() - 2], stack[stack.size() - 1]); //A B交換
						stack[stack.size() - 2]->setDenominator(stack[stack.size() - 1]->getDenominator()); // A 的值 給回A 
						stack[stack.size() - 2]->numerator = stack[stack.size() - 1]->numerator;
						stack[stack.size() - 2]->sign = stack[stack.size() - 1]->sign;

						stack[stack.size() - 2]->Divide(buffer);

					}
					else
					{
						stack[stack.size() - 2]->Divide(*stack[stack.size() - 1]);
					};
					stack.pop_back();
				}
			}
			else if (postfix[index] == '^')
			{
				if (stack.size() < 2)
				{
					BigNumber* error = new BigNumber();
					error->error = 2;
					return *error;
				}
				else {
					/* 當A不是小數 B是 則要swap 因為Class型態 */
					if (!stack[stack.size() - 2]->isDecimal() && stack[stack.size() - 1]->isDecimal())
					{
						BigDecimal buffer(*stack[stack.size() - 1]);
						//	給定分母
						buffer.setDenominator(stack[stack.size() - 1]->getDenominator());
						swap(stack[stack.size() - 2], stack[stack.size() - 1]);
						stack[stack.size() - 2]->setDenominator(stack[stack.size() - 1]->getDenominator());
						stack[stack.size() - 2]->numerator = stack[stack.size() - 1]->numerator;
						stack[stack.size() - 2]->sign = stack[stack.size() - 1]->sign;

						stack[stack.size() - 2]->Power(buffer);
					}
					else {

						stack[stack.size() - 2]->Power(*stack[stack.size() - 1]);
					};
					stack.pop_back();


				}
			}
			else if (postfix[index] == '!')
			{
				if (stack.size() < 1)
				{
					BigNumber* error = new BigNumber();
					error->error = 2;
					return *error;
				}
				else {
					if (!stack[stack.size() - 1]->isDecimal())
					{
						stack[stack.size() - 1]->Factorial();
					}
					else
					{
						BigNumber* error = new BigNumber();
						error->error = 4;
						return *error;
					}
				}
			};
		}
		else
		{
			NumRegister += postfix[index]; /* 只會加數字 */
		};
		
	}
	return *stack[0];
}