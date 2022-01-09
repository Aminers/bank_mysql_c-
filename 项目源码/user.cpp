#include <iostream>
#include "bank.h"
using namespace std;
/**
功能：设置用户信息
说明：二进制储存没有string类的接口，所以把string转为char[]数组
**/
void User::setUser(int acc, int pw, string ids, string nam, string adr,  double bal, int sta, long long pho) {
    account = acc;
    password = pw;
    id = ids;
    name = nam;
    addr = adr;
    balance = bal;
    status = sta;
    phoneNumber = pho;
}
