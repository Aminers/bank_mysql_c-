#pragma once
#ifndef MY_SQL_H
#define MY_SQL_H

#include<iostream>
#include<Windows.h>
#include<WinSock.h>
#include<mysql.h>

class DataBase
{
public:
	DataBase();
	~DataBase();
	//连接数据库 参数为ip 用户名 密码 数据库名 端口
	bool Connect(const char* ip, const char* name, const char* cypher, const char* database_name, const int port);
	//执行sql语句
	MYSQL_RES *Implement(const char* sentence);


private:
	bool _state;//连接状态 true为已连接
	MYSQL* _mysql;//mysql连接  
	MYSQL_RES* _res;//这个结构代表返回行的一个查询结果集  
	MYSQL_ROW _column;//一个行数据的类型安全(type-safe)的表示，表示数据行的列  
	char _query[450];//查询语句  
};

#endif // !MY_SQL_H
