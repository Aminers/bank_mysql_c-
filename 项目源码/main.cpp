#include <iostream>
#include <cstdio>
#include <direct.h>
#include "bank.h"
using namespace std;
DataBase* db;//数据库
MYSQL_RES* mysql_res;
MYSQL_ROW mysql_row;
char mysqlpwd[120],database[100];
char pwd[160];
int main() {
    //获取sql文件的绝对路径
    sprintf_s(pwd, "%s\\crebas.sql", _getcwd(NULL, 60));
    cout << "请输入mysql密码:" << endl;
    cin >> mysqlpwd;
    cout << "请输入连接的mysql数据库名:" << endl;
    cin >> database;
    ///连接数据库
    db = new DataBase;
    while (!db->Connect("localhost", "root", mysqlpwd, database, 3306)) {
        cout << "密码错误或连接不是数据库，请重新输入\n";
        cout << "请输入mysql密码:" << endl;
        cin >> mysqlpwd;
        cout << "请输入连接的mysql数据库名:" << endl;
        cin >> database;
    }
    char source_cmd[90];
    sprintf_s(source_cmd, "mysql -uroot -p%s %s<%s",mysqlpwd, database, pwd);
    system(source_cmd);//读取数据库文件

    Bank bank;
    bank.work();
    return 0;
}
