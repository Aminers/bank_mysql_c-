#include <iostream>
#include <fstream>
#include <stdio.h>
#include "bank.h"
extern DataBase* db;//数据库
using namespace std;
/**
功能：初始化链表
**/
Note::Note() {
    next = NULL;
}
/**
功能：从user类继承数据
**/
void Note::noteSet(User& t) {
    account = t.account;
    password = t.password;
    id = t.id;
    addr = t.addr;
    name = t.name;
    balance = t.balance;
    status = t.status;
    phoneNumber = t.phoneNumber;
}
/**
功能：把存/取款操作记录到日记
**/
void Note::add_save_record(double money) {
    ofstream noteFile("saveNote.txt",ios::app);
    noteFile << "用户:" << account << "\t存入" << money << "元\t\t时间:" << date << endl;
    noteFile.close();
    /*插入数据库 record表*/
    char cmd[260],dateT[240];
    for (int i = 0; i < date.size(); i++) dateT[i] = date[i]; dateT[date.size()] = '\0';
    sprintf_s(cmd, "insert into record(rdate,money,opt)\
                        values(\"%s\",%lf,%d)", dateT,money,0);//0表示存款
    db->Implement(cmd);
    sprintf_s(cmd, "insert into record_account(acc)\
                        values(%d)", account);
    db->Implement(cmd);
}
void Note::add_withdraw_record(double money) {
    ofstream noteFile("withdrawNote.txt", ios::app);
    noteFile << "用户:" << account << "\t取出" << money << "元\t\t时间:" << date << endl;
    noteFile.close();
    /*插入数据库 record表*/
    char cmd[260], dateT[240];
    for (int i = 0; i < date.size(); i++) dateT[i] = date[i]; dateT[date.size()] = '\0';
    sprintf_s(cmd, "insert into record(rdate,money,opt)\
                        values(\"%s\",%lf,%d)", dateT, money, 1);//1表示取款
    db->Implement(cmd);
    sprintf_s(cmd, "insert into record_account(acc)\
                        values(%d)", account);
    db->Implement(cmd);
}