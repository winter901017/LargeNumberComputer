#include "InfixToPostfix.h"
/*
Reference
https://www.tinytsunami.info/preorder-inorder-postorder/
*/

//	<0506> Priority Update
//		# from 3 increased to 6
int InStackPriority(char op)
{
	if (op == '+' || op == '-')return 1;
	else if (op == '*' || op == '/')return 2;
	else if (op == '#') return 6;
	else if (op == '^') return 4; //右邊到左邊運算 所以進入的優先權要大於輸入的優先權 <=
	else if (op == '!') return 5;
	else if (op == '(')return 7;
	else if (op == ')')return 0;
	return -1;
};

int OutStackPriority(char op)
{
	if (op == '+' || op == '-')return 1;
	else if (op == '*' || op == '/')return 2;
	else if (op == '#') return 6;
	else if (op == '^') return 3; //右邊到左邊運算 所以進入的優先權要大於輸入的優先權 <=
	else if (op == '!') return 5;
	else if (op == '(')return 0;
	//else if (op == ')')return 0;

	return -1;
};

bool isDigit(char c) {
	if (c >= 48 && c < 58)
	{
		return true;
	}
	return false;
}

bool isOperator(char op) {
	if (op == '(' || op == ')' || op == '!' || op == '^' || op == '+' || op == '-' || op == '*' || op == '/') {
		return true;
	}
	return false;
};

//Ture 是運算子壓得住stack
bool operatorPrecedenceCompare(char stackOp, char infixOp)
{
	if (OutStackPriority(stackOp) < InStackPriority(infixOp)) {
		return true;
	}
	else
	{
		return false;
	};
};
/*
num 
1 ( )
2 !   由左至右
3 ^   由右至左
4 +-  (正負號)由右至左
5 * / 由左至右
6 + - 由左至右
*/

string infixToPostfix(string infix)
{
	for (int i = 0; i < (int)infix.length(); i++)
	{
		if (infix[i] == ' ')
		{
			infix.erase(i, 1);
			i--;
		}
	}

	string stack; //運算子堆疊
	string postFix;
	bool blank = false; //區分運算元與運算子

	for (int index = 0; index < (int)infix.length(); index++)
	{
		if (isOperator(infix[index])) //判斷是不是運算子 (這樣寫可以將小數也判斷成數字 用數子判斷會有問題)
		{
			blank = false;

			//判斷正負符號
			if (infix[index] == '-' || infix[index] == '+')
			{
				if (index == 0 || (isOperator(infix[index - 1]) && infix[index - 1] != ')' && infix[index - 1] != '!')) //負號會出現的情況
				{
					if (infix[index] == '-') //負號要特別留著 要與減區分
					{
						stack.push_back('#');
					};
					continue;
				}
			}


			if (stack.empty()) //如果是空的就先放入 當底
			{
				stack.push_back(infix[index]);
				continue;
			}
			else if (infix[index] == '(')
			{
				stack.push_back(infix[index]);
			}
			else if (infix[index] == ')') //遇到右括號就一直pop 將資料放入postfix 直到發現 左括號 找到後再將左括號跟右括號消除
			{
				while (stack.back() != '(')
				{
					postFix += stack.back(); //將stack的運算子拿出來
					postFix += " "; 
					stack.pop_back(); //拿出來以後清除

				};
				stack.pop_back(); //消除左括號
			}

			//判斷新的運算子是否壓得住stack中的最後一個運算子
			//可以就 push 否則 pop直到壓得住
			else if (operatorPrecedenceCompare(stack.back(), infix[index])) { //如果壓得住 就放入運算子

					stack.push_back(infix[index]);

			}
			else //當壓不住運算子 則pop直到壓得住
			{
				if (stack.back() != '(' && stack.back() != ')') { //防止消除左括號以及右括號
						postFix += stack.back();
						postFix += " ";
						stack.pop_back();
				}; 
				index--; //再跑一次
			};

		}
		else {
				if (blank) //上一個push是運算元 當下的值還是浮點數或是整數
				{
					postFix.pop_back();
				}

				postFix += infix[index];
				postFix += " ";

				blank = true; //代表有空格過了 且 是運算元
		};

	}

	while (!stack.empty())
	{
		postFix += stack.back();
		postFix += " ";
		stack.pop_back();
	}

	return postFix;
};
