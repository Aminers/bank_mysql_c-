#include <iostream>
#include <fstream>
#include <conio.h>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <ctime>
#include <sstream>
#include <iomanip>
#include "bank.h"
extern DataBase* db;//数据库
extern MYSQL_RES* mysql_res;
extern MYSQL_ROW mysql_row;
/**
功能：验证用户password
**/
bool pass(const char *sName,const char *sPass) {
    /*从数据库获取业务员账号密码*/
    mysql_res=db->Implement("select * from admin_login");///查询所有数据
    int countStuff = mysql_num_rows(mysql_res);
    for (int i = 0; i < countStuff; i++) {
        mysql_row = mysql_fetch_row(mysql_res);
        if (strcmp(sName, mysql_row[0]) == 0) return strcmp(sPass, mysql_row[1]) == 0;
    }
    mysql_free_result(mysql_res);
    return false;
}
/**
功能：获得当前时间
**/
string getTime() {
    const time_t t = time(0);
    tm now;
    localtime_s(&now,&t);
    int y = now.tm_year + 1900;
    int m = now.tm_mon + 1;
    int d = now.tm_mday;
    ostringstream ostr;
    ostr << y << "-" << setw(2) << setfill('0') << m << "-" << d;
    return ostr.str();
}