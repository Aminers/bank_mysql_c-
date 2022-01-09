#include "mysql.h"
#include<iostream>
using namespace std;

DataBase::DataBase()
{
    _state = false;
    _mysql = new MYSQL;
    _res = nullptr;
    _column = nullptr;
    memset(_query, 0, sizeof(_query));
}

DataBase::~DataBase()
{
    mysql_close(_mysql);
    delete this;//释放连接
}

bool DataBase::Connect(const char* ip, const char* name, const char* cypher, const char* database_name, const int port)
{
    if (true == _state)
    {
        printf("Database connected\n");
        return false;
    }
    //初始化mysql  
    mysql_init(_mysql);
    //返回false则连接失败，返回true则连接成功  
    if (!(mysql_real_connect(_mysql, ip, name, cypher, database_name, port, NULL, 0))) //中间分别是主机，用户名，密码，数据库名，端口号（可以写默认0或者3306等），可以先写成参数再传进去  
    {
        printf("Error connecting to database:%s\n", mysql_error(_mysql));
        return false;
    }
    else
    {
        _state = true;
        printf("Connected succeed\n\n");
        return true;
    }
    return true;
}

MYSQL_RES* DataBase::Implement(const char* sentence)
{
    if (false == _state)
    {
        printf("Database not connected\n");
        return NULL;
    }
    //查询内容
    sprintf_s(_query, "%s", sentence); //desc 语句获取字段数
    //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码 
    mysql_query(_mysql, "SET NAMES UTF8");
    //执行SQL语句
    if (mysql_query(_mysql, _query))
    {
        printf("Query failed (%s)\n", mysql_error(_mysql));
        return NULL;
    }
    //获取结果集  
    if (!(_res = mysql_store_result(_mysql)))   //获得sql语句结束后返回的结果集 
    {
        return NULL;
    }
    return _res;
}
