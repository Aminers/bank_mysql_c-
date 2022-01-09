#pragma once
#ifndef BANK_H_INCLUDED
#define BANK_H_INCLUDED
#include <string>
#include <map>
#include "mysql.h"
using namespace std;
const int upNum = 2000; ///系统最多容纳的用户数
class Bank;
class Note;
class User {
public:
    void setUser(int acc, int pw, string ids, string nam, string adr, double bal, int sta, long long pho);
    friend class Bank;   ///将Bank声明为友元类，方便其訪问数据成员
    friend class Note;
protected:
    int account; ///账号
    int password;  ///password
    string id;  ///身份证
    string name;   ///username
    string addr;   ///地址
    double balance;  ///账户剩余金额
    long long phoneNumber;///手机号码
    int status;  ///状态 0-正常  1-销户
};
class Note :public User {
public:
    friend class Bank;
    Note();///构造函数
    Note* next;
    void noteSet(User& t);///用user对象给note赋值
    void add_save_record(double money);
    void add_withdraw_record(double money);
private:
    string date;
};
class Bank {
public:
    Bank();  ///開始前从文件里读数据，存在数组中
    ~Bank();   ///程序结束前，将数组中的数据写入到文件里
    static void work();  ///业务驱动
    static int openAccount(const char* iPass1, const char* iPass2, const char* id,
        const char* nam, const char* addr, const char* phone1, const char* phone2, const char* bal); ///开户
    static int cancelAccount(const char* account, const char* pas);  ///注销账户
    static int save(const char* account, const char* bal);   ///存款
    static int withdraw(const char* account, const char* pas, const char* bal);   ///取款
    static int showAccount(const char* account, const char* pas); ///查询剩余金额
    static int transferAccounts(const char* account, const char* pas, const char* bal, const char* account2);  ///转账
    static int updatePassword(const char* account, const char* oldPas, const char* newPas1, const char* newPas2);   ///更改password
    static int updatePassword_phone(const char* account, const char* phone, const char* newPas1, const char* newPas2);
    static int bindingPhonenumber(const char* account, const char* pas, const char* phone1, const char* phone2); ///绑定手机号
    static Note* getUser(int acc);  ///输入账号查询用户
    static void add(User a);///加入链表
    static int nowAcc();//返回当前开户的账号
private:
    static int N;  ///实际的用户数目
    static Note* head;///链表储存用户信息
    static int newAcc;//新账号的号码
};
bool pass(const char* sName, const char* sPass);  ///业务员登录
long long checkNum(const char* pas,const int w);//检查密码是否是w个数字,w=-1则不要求位数，但要是全是数字。返回数字，-1表示输入错误
string getTime();//获得当前时间 年份-月份-日期
#endif // BANK_H_INCLUDED