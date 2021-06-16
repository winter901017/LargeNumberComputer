// Name: 李宸緯
// Date: May 5, 2021  
// Last Update: March 5, 2021  
// Problem statement: 大數計算機

#include <iostream>
#include <string>
#include <vector>
#include "InfixToPostfix.h" //算數 運算子優先權 將中序轉換成後序
#include "PostfixCompute.h"
#include "BigDecimal.h"

//#define API1
//#define API2
//#define API3

//#define 
using namespace std;

int main()
{
#ifdef API1
    while (1)
    {
        BigDecimal i;
        BigNumber d;

        cin >> i >> d;
        cout << "i   = " << i << endl;
        cout << "d   = " << d << endl;
        cout << "加 = " << i + d << endl;
        cout << "減  = " << i - d << endl;
        cout << "乘 = " << i * d << endl;
        cout << "除 = " << i / d << endl;
    };
#endif

#ifdef API2
    BigNumber i = "12345";
    BigDecimal d = "0.3 * 3";
    string output1 = i.print();
    string output2 = d.print();
    cout << output1 << endl;
    cout << output2 << endl;

   
#endif 

#ifdef API3
    BigNumber i = "123*10";
    BigDecimal d = "123.3";
    vector<BigNumber*> nums;
    nums.push_back(&i);
    nums.push_back(&d);
    for (const auto& num : nums)
        cout << num->print() << endl;
#endif // API3

  
    string str;
    while (1) {
        string ans; //輸出結果

        getline(cin, str); //讀整行分析資料  到時候要做除錯之類的....
        if (str == "")continue;
        /* 判斷一下是不是 Set Integer or Set Decimal */
        /*
            設定 Set Integer A = 3.5
                 Set Decimal A = 1.0
                 012345678901234
                 15個字才開始判斷
        */
        /* 如果開頭是空白 刪除空白*/
        while (str[0] == ' ')
        {
            str.erase(0, 1);
        }

        /* 找等於 變數 A= A + 1 的特殊處理 */
        auto itEqual = str.find('='); //找到等於的index
        auto itSet = str.find("Set"); //防止誤判 Set 等式
        if (itSet == string::npos && itEqual != string::npos) //字串要有=不能有Set 防止誤判 Set
        {    
            string name, expression;
            for (int i = 0; i < (int)str.length(); i++)
            {
                if (i < (int)itEqual)
                {
                    name += str[i];
                }
                else if (i > (int)itEqual)
                {
                    expression += str[i];
                };
            }
            while (name.back() == ' ') //清除變數後面的空白 防呆
            {
                name.pop_back();
            }
            auto itInteger = BigNumber::bigNumbers.find(name);
            auto itDecimal = BigDecimal::bigDecimals.find(name);
            bool isFound = false;

            if (itDecimal != BigDecimal::bigDecimals.end())
            {
                string assign, tmp;
                tmp = infixToPostfix(expression); //將中序轉換成後序
                assign = PostfixCompute(tmp).print(); //計算結果

                BigDecimal::bigDecimals.erase(name); //刪除重新定義             
                BigDecimal::bigDecimals[name] = BigDecimal(assign); //重新定義

                isFound = true;
            }
            else if (isFound == false && itInteger != BigNumber::bigNumbers.end())
            {
                string assign, tmp;
                tmp = infixToPostfix(expression); //將中序轉換成後序
                assign = PostfixCompute(tmp).print(); //計算結果
                /* 整數 要特別去小數點 */
                for (int i = 0; i < (int)assign.size(); i++)
                {
                    if (assign[i] == '.') // 找小數點
                    {
                        while (assign[i] == '.')
                        {
                            assign.pop_back();
                        }
                        break;
                    };
                }
                BigNumber::bigNumbers.erase(name); //刪除重新定義
                BigNumber::bigNumbers[name] = BigNumber(assign); //重新定義

               // stack.push_back(expression);
            }
        }
        else {
            string type, name, expression;
            /* 變數設置 判斷 開頭是英文 還有大小要大於15至少要超過等於 */
            if ((str[0] >= 'a' && str[0] <= 'z') || (str[0] >= 'A' && str[0] <= 'Z') && (int)str.length() > 15) //
            {
                type = str.substr(0, 11);
                bool findEqual = false;
                bool isPoint = false;
                for (int i = 0; i < (int)str.size(); i++)
                {
                    if (str.size() > 15) {

                        /* 尋找變數名稱 */
                        if (i > 11 && !findEqual && str[i] != ' ' && str[i] != '=')
                        {
                            name += str[i];
                        };
                        /* 表達式 & 找小數點 */
                        if (findEqual) {
                            if (str[i] == '.')
                            {
                                isPoint = true;
                            }
                            expression += str[i];
                        }
                        /* 判斷有沒有找到等於 找到後 後面是表達式 */
                        if (str[i] == '=')
                        {
                            findEqual = true;
                            i++;
                        }

                    };
                }
                if (type == "Set Integer")
                {
                    if (isPoint) //消除整式後面的小數點
                    {
                        while (expression.back() != '.')
                        {
                            expression.pop_back(); //清掉小數點後面的數字
                        }
                        expression.pop_back(); //清掉小數點
                    }
                    auto it = BigNumber::bigNumbers.find(name);
                    if (it != BigNumber::bigNumbers.end())
                    {
                        BigNumber::bigNumbers.erase(name); //刪除重新定義
                    }
                    BigNumber::bigNumbers[name] = BigNumber(expression);


                }
                else if (type == "Set Decimal")
                {
                    auto it = BigDecimal::bigDecimals.find(name);

                    if (it != BigDecimal::bigDecimals.end())
                    {
                        BigDecimal::bigDecimals.erase(name); //刪除重新定義
                    }
                    BigDecimal::bigDecimals[name] = BigDecimal(expression);

                }
                else
                {
                    cout << "輸入格式錯誤\n";
                }
            }
            else {
                string postFix; //後序
                postFix = infixToPostfix(str); //將中序轉換成後序
                cout << postFix << endl; // debug
                ans = PostfixCompute(postFix).print();
                cout << ans << endl;
            };
        };
    };
    return 0;
}