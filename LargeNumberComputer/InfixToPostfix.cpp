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
	else if (op == '^') return 4; //�k��쥪��B�� �ҥH�i�J���u���v�n�j���J���u���v <=
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
	else if (op == '^') return 3; //�k��쥪��B�� �ҥH�i�J���u���v�n�j���J���u���v <=
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

//Ture �O�B��l���o��stack
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
2 !   �ѥ��ܥk
3 ^   �ѥk�ܥ�
4 +-  (���t��)�ѥk�ܥ�
5 * / �ѥ��ܥk
6 + - �ѥ��ܥk
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

	string stack; //�B��l���|
	string postFix;
	bool blank = false; //�Ϥ��B�⤸�P�B��l

	for (int index = 0; index < (int)infix.length(); index++)
	{
		if (isOperator(infix[index])) //�P�_�O���O�B��l (�o�˼g�i�H�N�p�Ƥ]�P�_���Ʀr �μƤl�P�_�|�����D)
		{
			blank = false;

			//�P�_���t�Ÿ�
			if (infix[index] == '-' || infix[index] == '+')
			{
				if (index == 0 || (isOperator(infix[index - 1]) && infix[index - 1] != ')' && infix[index - 1] != '!')) //�t���|�X�{�����p
				{
					if (infix[index] == '-') //�t���n�S�O�d�� �n�P��Ϥ�
					{
						stack.push_back('#');
					};
					continue;
				}
			}


			if (stack.empty()) //�p�G�O�Ū��N����J ��
			{
				stack.push_back(infix[index]);
				continue;
			}
			else if (infix[index] == '(')
			{
				stack.push_back(infix[index]);
			}
			else if (infix[index] == ')') //�J��k�A���N�@��pop �N��Ʃ�Jpostfix ����o�{ ���A�� ����A�N���A����k�A������
			{
				while (stack.back() != '(')
				{
					postFix += stack.back(); //�Nstack���B��l���X��
					postFix += " "; 
					stack.pop_back(); //���X�ӥH��M��

				};
				stack.pop_back(); //�������A��
			}

			//�P�_�s���B��l�O�_���o��stack�����̫�@�ӹB��l
			//�i�H�N push �_�h pop�������o��
			else if (operatorPrecedenceCompare(stack.back(), infix[index])) { //�p�G���o�� �N��J�B��l

					stack.push_back(infix[index]);

			}
			else //��������B��l �hpop�������o��
			{
				if (stack.back() != '(' && stack.back() != ')') { //����������A���H�Υk�A��
						postFix += stack.back();
						postFix += " ";
						stack.pop_back();
				}; 
				index--; //�A�]�@��
			};

		}
		else {
				if (blank) //�W�@��push�O�B�⤸ ��U�����٬O�B�I�ƩάO���
				{
					postFix.pop_back();
				}

				postFix += infix[index];
				postFix += " ";

				blank = true; //�N���Ů�L�F �B �O�B�⤸
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
