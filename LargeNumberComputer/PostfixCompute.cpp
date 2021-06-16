#include "PostfixCompute.h"
#include "InfixToPostfix.h"
#include "BigNumber.h"
#include "BigDecimal.h"
#include <vector>
BigNumber& PostfixCompute(string postfix) {
	/*
	�Q�k:
	�@�� ans �� ������� 
	�@�� NumRegister �� �ȮɹB�⤸
	�]�^�� ���P�_�O���O�B�⤸ �_�h���NumRegister�̷�J��Ů� �N����
	NumRegister�M��
	���p�U�Ӥ@�ˬO �B�⤸ �h�@��
	�p�G�J��B��l �ݬO�n��@�ӼƦr�άO��ӼƦr�Ӱ��B��
	*/
	string ans;
	vector<BigNumber*> stack; //��J�j�ƪ��B�⤸  /* �]��virtual �P �~�� �ҥH�n��pointer */
	string NumRegister; //�Ʀr�Ȧs��

	//int error = 0; 
	// error 1 size�p��2 �⦡���~


	for (int index = 0; index < (int)postfix.length(); index++)
	{
		if (postfix[index] == ' ') // ��ܦ��B�⤸�ιB��l
		{
			if (NumRegister != "")
			{
				/* �P�_�O�_�O�ܼ� �n��o�ন��F��*/
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
						//	�X�{���ŧi�ܼ�
						BigNumber* error = new BigNumber();
						error->error = 1;
						return *error;
					}
				}
				else {

					/* �n�P�_�O����٬O�B�I�� */
					// �p���I���u�@�ӬO��
					int pointCount = 0;
					for (int i = 0; i < (int)NumRegister.size(); i++)
					{
						if (NumRegister[i] == '.') // ��p���I
						{
							pointCount++;
						};
					}
					if (pointCount == 0) // �S���p���I
					{
						BigNumber* push = new BigNumber(NumRegister);
						stack.push_back(push);
					}
					else if (pointCount == 1) // ���@�Ӥp���I
					{
						BigNumber* push = new BigDecimal(NumRegister);
						stack.push_back(push);
					}
					else { // ���u�@�Ӥp���I error
						/* �]�����~��J �n�令return�O���F�� �_�h�| vector range*/
						BigNumber* error = new BigNumber();
						error->error = 2;
						return *error;
					}
				};
				NumRegister = ""; //�M�żȦs�B�⤸
			}
			continue;
		}

		/* �H�U�O�B��l���p�� */
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
				//�n�P�_ ���t�� �P �O���O�B�I�� �b���B�� ��operator overloading���B��
				if (stack.size() < 2) 
				{
					BigNumber* error = new BigNumber();
					error->error = 2;
					return *error;
				}
				else 
				{
					/* ��A���O�p�� B�O �h�nswap �]��Class���A */
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
				//�n�P�_ ���t�� �P �O���O�B�I�� �b���B�� ��operator overloading���B��
				if (stack.size() < 2) 
				{
					BigNumber* error = new BigNumber();
					error->error = 2;
					return *error;
				}
				else {

					/* ��A���O�p�� B�O �h�nswap �]��Class���A */
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
					/* ��A���O�p�� B�O �h�nswap �]��Class���A */
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
					/* ��A���O�p�� B�O �h�nswap �]��Class���A */
					if (!stack[stack.size() - 2]->isDecimal() && stack[stack.size() - 1]->isDecimal())
					{
						BigDecimal buffer;
						buffer.numerator = stack[stack.size()-1]->numerator;
						buffer.sign = stack[stack.size() - 1]->sign;
						buffer.setDenominator(stack[stack.size() - 1]->getDenominator()); //Buferrr �O B ������buffer�Ӱ�
						swap(stack[stack.size() - 2], stack[stack.size() - 1]); //A B�洫
						stack[stack.size() - 2]->setDenominator(stack[stack.size() - 1]->getDenominator()); // A ���� ���^A 
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
					/* ��A���O�p�� B�O �h�nswap �]��Class���A */
					if (!stack[stack.size() - 2]->isDecimal() && stack[stack.size() - 1]->isDecimal())
					{
						BigDecimal buffer(*stack[stack.size() - 1]);
						//	���w����
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
			NumRegister += postfix[index]; /* �u�|�[�Ʀr */
		};
		
	}
	return *stack[0];
}