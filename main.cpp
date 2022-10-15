#include <string>
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <vector>
#include <stack>
#include <cmath>
using namespace std;

double toNum (double c)
{
    return c - double('0');
}

double toChar (double a)
{
    return a + double('0');
}

bool isNum(double c)
{
    return (toNum(c)>=0);
}

bool findEqual(string str)
{
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i]=='=')
        {
            return true;
        }
    }
    return false;
}

int findCharacter(vector<char> l,char c)
{
    for (int i = 0; i < l.size(); i++)
    {
        if(l[i]==c)
            return i+1;
    }
    return 0;
}


int main()
{   
    string inp;
    cin>>inp;
    vector<char> character;
    vector<double> number;
    double v;
    while(findEqual(inp)) //判断是否有变量，有的话记录便量的字母以及变量的值
    {   
        character.push_back(inp[0]);
        v = 0;
        for (int i = 0; i < inp.length()-2; i++)
        {
            v+=toNum(inp[inp.length()-1-i]) * int(pow(10,i));
        }
        number.push_back(v);
        cin>>inp;
    }

    vector<double> expr;
    int ind1 = 0;
    int ind2 = 0;
    int ind3 = 0;
    int temp = 0;
    int count =0;
    int index1;
    int index2;

    while (inp.find("sqrt")<100) //判断输入字符串中是否有sqrt()的表达，有的话将其替换为一定的变量，并记录变量的值
    {   
        temp=0;
        index1 = inp.find("sqrt");
        for (int i = index1; i < inp.length(); i++)
        {
            if (inp[i]==')')
                {
                    index2=i;
                    break;
                }
        }
        for (int j = 0; j < index2-index1-5; j++)
        {
            temp += toNum(inp[index2-j-1]) * int(pow(10,j));
        }
        character.push_back(('O'+count));
        number.push_back(sqrt(temp));
        inp[index1]=('O'+count);
        inp.erase(index1+1,index2-index1);
        count++;
    }

    //将字符串转换为vector形式，形成中缀表达式，即一个元素为整个数字或者运算符
    for (int i = 0; i <inp.length(); i++)
    {
        if(findCharacter(character,inp[i])!=0) 
        {
            expr.push_back(toChar(number[findCharacter(character,inp[i])-1]));
            continue;
        }
        else
        {   
            if (!isNum(inp[i])&&(findCharacter(character,inp[i-1])!=0))
            {
                expr.push_back(inp[i]);
                ind1 = i+1;
            }
            
            else if (i==inp.length()-1 && isNum(inp[i]))
            {   
                temp = 0;
                for (int j = 0; j < i-ind1+1; j++)
                    {
                        temp += toNum(inp[i-j]) * int(pow(10,j));
                    }
                    expr.push_back(toChar(temp)); 
            }
            else if (!isNum(inp[i]))
            {
                ind2 =i;
                if (isNum(inp[i-1]))
                {
                    temp = 0;
                    for (int j = 0; j < ind2-ind1; j++)
                    {
                        temp += toNum(inp[ind2-1-j]) * int(pow(10,j));
                    }
                    expr.push_back(toChar(temp)); 
            
                }
                expr.push_back(inp[i]);
                ind1 = i+1;
            }
            
        }
    }


    //将上面生成的中缀表达式转换为后缀表达式
    vector<double> hexpr;
    stack<double> oper;
    for (int i = 0; i < expr.size(); i++)
    {
        if (isNum(expr[i]))
            hexpr.push_back(expr[i]);
        else
        {
            if (oper.empty())
                oper.push(expr[i]);
            else if (expr[i]=='(')
                oper.push(expr[i]);
            else if (expr[i]==')')
            {
                while(oper.top()!='(')
                {
                    hexpr.push_back(oper.top());
                    oper.pop();
                }
                oper.pop();
            }
            else if (expr[i]=='*' || expr[i]=='/')
            {
                if (oper.top()=='(' || oper.top()=='+' || oper.top()=='-' || oper.empty())
                {
                    oper.push(expr[i]);
                }
                else if (oper.top()=='*' || oper.top()=='/')
                {
                    hexpr.push_back(oper.top());
                    oper.pop();
                    oper.push(expr[i]);
                }
            }
            else if (expr[i]=='+' || expr[i]=='-')
            {
                while(true)
                {
                    if(oper.empty() || oper.top()=='(')
                    {
                        oper.push(expr[i]);
                        break;
                    }
                    else if(oper.top()=='*' || oper.top()=='/')
                    {
                        hexpr.push_back(oper.top());
                        oper.pop();
                    }
                    else if(oper.top()=='+' || oper.top()=='-')
                    {
                        hexpr.push_back(oper.top());
                        oper.pop();
                        oper.push(expr[i]);
                        break;
                    }
                }
            }  
        }
    }
    while (!oper.empty())
    {
        hexpr.push_back(oper.top());
        oper.pop();
    }
    
    //后缀表达式计算
    vector<double> tempV;
    double tem;
       for (int i = 0; i < hexpr.size(); i++)
        {
            if (isNum(hexpr[i]))
            {
                tempV.push_back(hexpr[i]);
            }
            else if (hexpr[i]=='+')
            {   
                tem = toChar(toNum(tempV[tempV.size()-2])+toNum(tempV[tempV.size()-1]));
                tempV.pop_back();
                tempV.pop_back();
                tempV.push_back(tem);
            }
            else if (hexpr[i]=='-')
            {   
                tem = toChar(toNum(tempV[tempV.size()-2])-toNum(tempV[tempV.size()-1]));
                tempV.pop_back();
                tempV.pop_back();
                tempV.push_back(tem);
            }
            else if (hexpr[i]=='*')
            {   
                tem = toChar(toNum(tempV[tempV.size()-2])*toNum(tempV[tempV.size()-1]));
                tempV.pop_back();
                tempV.pop_back();
                tempV.push_back(tem);
            }
            else if (hexpr[i]=='/')
            {   
                tem = toChar(toNum(tempV[tempV.size()-2])/toNum(tempV[tempV.size()-1]));
                tempV.pop_back();
                tempV.pop_back();
                tempV.push_back(tem);
            }   
        }
    cout << toNum(tempV[0]) <<endl;
    
    return 0;
}