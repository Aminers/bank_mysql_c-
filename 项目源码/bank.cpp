#include <iostream>
#include <fstream>
#include <cstdlib>
#include "bank.h"
#include "window.h"
#include <ctype.h>
extern DataBase* db;//数据库
extern MYSQL_RES* mysql_res;
extern MYSQL_ROW mysql_row;
extern char mysqlpwd[20];
extern char pwd[60],database[100];
extern Fl_Window* windows[11];
extern Fl_Button* okBtn[10], * goBtn[8], * backBtn[9], * forgetBtn;
char okBuf[10] = "确认";
char backBuf[10] = "回退";
char fBuf[20] = "忘记密码";
using namespace std;
int Bank::N=0;  ///实际的用户数目
Note* Bank::head=NULL;///链表储存用户信息
int Bank::newAcc=1;//新账号的号码
map<int, bool> used;//已经用过的账号名
Fl_Input* sNames, *sPasss;
Fl_Input* oAIn[8],*cAIn[2],*sIn[2],*wIn[3],*srIn[2],*tIn[4],*uIn[8],*bIn[4];
Fl_Box* box[9];
/**
功能：用构造函数读取文件里保存的业务数据
**/
Bank::Bank() {
    /*从数据库读取数据*/
    mysql_res=db->Implement("select a.acc,a.passwd,b.id,b.name,b.addr,a.balance,b.phone,a.state\
                        from account a,user b,account_id c\
                        where a.acc = c.acc and b.id = c.id");
    int account; ///账号
    int password;  ///password
    string id;  ///身份证
    string name;   ///username
    string addr;   ///地址
    double balance;  ///账户剩余金额
    long long phoneNumber;///手机号码
    int status;  ///状态 0-正常  1-销户
    User p;used.clear();
    if(mysql_res!=NULL) N = mysql_num_rows(mysql_res); 
    for (int i = 0; i < N;i++) {
        mysql_row=mysql_fetch_row(mysql_res);
        //cout << "ms:" << (mysql_row == NULL) << endl;
        //for (int j = 0; j < 8; j++) cout << mysql_row[j] << endl;
        account = atoi(mysql_row[0]);
        password = atoi(mysql_row[1]);
        id = mysql_row[2];
        name = mysql_row[3];
        addr = mysql_row[4];
        balance = atof(mysql_row[5]);
        phoneNumber = atoll(mysql_row[6]);
        status = atoi(mysql_row[7]);
        p.setUser(account, password, id, name, addr, balance, status, phoneNumber);
        add(p);
        used[account] = 1;
    }
    if (mysql_res != NULL) mysql_free_result(mysql_res);
}
/**
功能：用析构函数，在退出系统时。将数据保存到文件里
**/
Bank::~Bank() {
    Note* data;
    ///用于直观的观看数据
    ofstream outfile("seeaccount.txt", ios::out);
    ///測试文件打开操作是否成功。不成功则提示后退出。
    if (!outfile) {
        cerr << "open error3!" << endl;
        exit(1);
    }
    data = head;
    while (data != NULL) {
        outfile << "account: " << data->account << "\t";
        outfile << "name: " << data->name << "\t";
        outfile << "id: " << data->id << "\t";
        outfile << "addr: " << data->addr << "\t";
        outfile << "password: " << data->password << "\t";
        outfile << "balance: " << data->balance << "\t";
        outfile << "status: " << data->status << "\t";
        outfile << "phoneNumber: " << data->phoneNumber << "\n";
        data = data->next;
    }
    outfile.close();
    ///将数据保存到数据库中
    char id[50],name[50],addr[50],cmd[460];
    memset(id, 0, sizeof id); memset(name, 0, sizeof name); memset(addr, 0, sizeof addr); memset(cmd, 0, sizeof cmd);
    data = head;
    while (data != NULL) {
        //string转char**
        for (int i = 0; i < data->id.size(); i++) id[i] = data->id[i]; id[data->id.size()] = '\0';
        for (int i = 0; i < data->name.size(); i++) name[i] = data->name[i]; name[data->name.size()] = '\0';
        for (int i = 0; i < data->addr.size(); i++) addr[i] = data->addr[i]; addr[data->addr.size()] = '\0';

        //如果账号已被注销，删除数据库的数据
        if (data->status == 1) {
            //account表
            sprintf_s(cmd, "delete from %s\
                        where acc=%d", "account", data->account);
            db->Implement(cmd);
            //account_id表
            sprintf_s(cmd, "delete from %s\
                        where acc=%d", "account_id", data->account);
            db->Implement(cmd);
            //record_account表
            sprintf_s(cmd, "delete from %s\
                        where acc=%d", "record_account", data->account);
            db->Implement(cmd);
        }
        else {
            //插入数据库的表格
            //插入account表
            sprintf_s(cmd, "select count(*) from account where acc=%d", data->account);
            mysql_res = db->Implement(cmd);
            mysql_row = mysql_fetch_row(mysql_res);
            if (strcmp(mysql_row[0],"0")==0) {//不存在该数据，直接插入
                sprintf_s(cmd, "insert into %s\
                                values(%d,%d,%d,%lf)", "account",data->account,data->password,data->status,data->balance);
                db->Implement(cmd);
            }
            else {//已存在该数据，update更新
                sprintf_s(cmd, "update account set\
                                passwd=%d,state=%d,balance=%lf\
                                where acc=%d", data->password, data->status, data->balance, data->account);
                db->Implement(cmd);
            }
            
            //插入user表
            sprintf_s(cmd, "select count(*) from user where id=\"%s\"", id);
            mysql_res = db->Implement(cmd);
            mysql_row = mysql_fetch_row(mysql_res);
            if (strcmp(mysql_row[0], "0") == 0) {//不存在该数据，直接插入
                sprintf_s(cmd, "insert into %s\
                            values(\"%s\",\"%s\",\"%s\",%lld)", "user", id, name, addr, data->phoneNumber);
                db->Implement(cmd);
            }
            else {//已存在该数据，update更新
                sprintf_s(cmd, "update user set\
                                name=\"%s\",addr=\"%s\",phone=%lld\
                                where id=\"%s\"", name, addr, data->phoneNumber, id);
                db->Implement(cmd);
            }
            

            //插入account_id表
            sprintf_s(cmd, "select count(*) from account_id where acc=%d", data->account);
            mysql_res = db->Implement(cmd);
            mysql_row = mysql_fetch_row(mysql_res);
            if (strcmp(mysql_row[0], "0") == 0) {//不存在该数据，直接插入
                sprintf_s(cmd, "insert into %s\
                            values(%d,\"%s\")", "account_id", data->account, id);
                db->Implement(cmd);
            }
            else {//已存在该数据，update更新
                sprintf_s(cmd, "update account_id set\
                                id=\"%s\"\
                                where acc=%d", id, data->account);
                db->Implement(cmd);
            }
            
        }
        data = data->next;
    }
    mysql_free_result(mysql_res);

    ///删除动态分配的空间，摧毁链表。
    Note* q, * p = head;
    while (p != NULL) {
        q = p->next;
        delete (p);
        p = q;
    }

    char exCmd[90];
    sprintf_s(exCmd, "mysqldump %s>%s -uroot -p%s", database, pwd ,mysqlpwd);
    system(exCmd);//备份数据库
}
/**
功能：办理业务窗口
**/
void Bank::work() {
    ///办理业务
    for (int i = 10; i > 0; i--) windows[i] = new Fl_Window(300, 450);
    windows[0] = new Fl_Window(400, 300);

    /*windows[0]，登录窗口*/
    //输入业务员账号和密码
    sNames = new Fl_Input(180, 80, 150, 50, "业务员账号(默认admin):");
    sPasss = new Fl_Input(180, 160, 150, 50, "业务员密码(默认123456):");
    //登录界面按钮
    okBtn[9] = new Fl_Button(330, 240, 65, 25, "&确认");
    okBtn[9]->callback((Fl_Callback*)Btn0login_cb,okBuf);
    windows[0]->show();

    /*windows[1]，选择窗口*/
    //选择界面,8个功能按钮  开户，销户，存款，取款，查询，转账，绑定，改密
    char buf1[10] = "开户";
    goBtn[0] = new Fl_Button(30, 50, 80, 50, "&开户");
    goBtn[0]->callback((Fl_Callback*)Btn2open_cb, buf1);
    char buf2[10] = "销户";
    goBtn[1] = new Fl_Button(190, 50, 80, 50, "&销户");
    goBtn[1]->callback((Fl_Callback*)Btn3open_cb, buf2);
    char buf3[10] = "存款";
    goBtn[2] = new Fl_Button(30, 150, 80, 50, "&存款");
    goBtn[2]->callback((Fl_Callback*)Btn4open_cb, buf3);
    char buf4[10] = "取款";
    goBtn[3] = new Fl_Button(190, 150, 80, 50, "&取款");
    goBtn[3]->callback((Fl_Callback*)Btn5open_cb, buf4);
    char buf5[10] = "查询";
    goBtn[4] = new Fl_Button(30, 250, 80, 50, "&查询");
    goBtn[4]->callback((Fl_Callback*)Btn6open_cb, buf5);
    char buf6[10] = "转账";
    goBtn[5] = new Fl_Button(190, 250, 80, 50, "&转账");
    goBtn[5]->callback((Fl_Callback*)Btn7open_cb, buf6);
    char buf7[10] = "绑定";
    goBtn[6] = new Fl_Button(30, 350, 80, 50, "&绑定");
    goBtn[6]->callback((Fl_Callback*)Btn8open_cb, buf7);
    char buf8[10] = "改密";
    goBtn[7] = new Fl_Button(190, 350, 80, 50, "&改密");
    goBtn[7]->callback((Fl_Callback*)Btn9open_cb, buf8);
    for (int i = 0; i < 8; i++) windows[1]->add(goBtn[i]);

    /*windows[2~10]各模块窗口*/
    //底部的确认按钮和返回按钮
    for (int i = 0; i < 9; i++) {
        okBtn[i] = new Fl_Button(60, 415, 65, 25, "&确认");
        backBtn[i] = new Fl_Button(175, 415, 65, 25, "&回退");
        windows[i + 2]->add(okBtn[i]);
        windows[i + 2]->add(backBtn[i]);
    }
    okBtn[0]->callback((Fl_Callback*)Btn2ok_cb, okBuf);
    backBtn[0]->callback((Fl_Callback*)Btn2close_cb, backBuf);
    okBtn[1]->callback((Fl_Callback*)Btn3ok_cb, okBuf);
    backBtn[1]->callback((Fl_Callback*)Btn3close_cb, backBuf);
    okBtn[2]->callback((Fl_Callback*)Btn4ok_cb, okBuf);
    backBtn[2]->callback((Fl_Callback*)Btn4close_cb, backBuf);
    okBtn[3]->callback((Fl_Callback*)Btn5ok_cb, okBuf);
    backBtn[3]->callback((Fl_Callback*)Btn5close_cb, backBuf);
    okBtn[4]->callback((Fl_Callback*)Btn6ok_cb, okBuf);
    backBtn[4]->callback((Fl_Callback*)Btn6close_cb, backBuf);
    okBtn[5]->callback((Fl_Callback*)Btn7ok_cb, okBuf);
    backBtn[5]->callback((Fl_Callback*)Btn7close_cb, backBuf);
    okBtn[6]->callback((Fl_Callback*)Btn8ok_cb, okBuf);
    backBtn[6]->callback((Fl_Callback*)Btn8close_cb, backBuf);
    okBtn[7]->callback((Fl_Callback*)Btn9ok_cb, okBuf);
    backBtn[7]->callback((Fl_Callback*)Btn9close_cb, backBuf);
    okBtn[8]->callback((Fl_Callback*)Btn10ok_cb, okBuf);
    backBtn[8]->callback((Fl_Callback*)Btn10close_cb, backBuf);

    /*开户模块*/
    oAIn[0] = new Fl_Input(85, 20, 150, 30, "*密码:");
    oAIn[1] = new Fl_Input(85, 70, 150, 30, "*确认密码:");
    oAIn[2] = new Fl_Input(85, 120, 150, 30, "*身份证:");
    oAIn[3] = new Fl_Input(85, 170, 150, 30, "姓名:");
    oAIn[4] = new Fl_Input(85, 220, 150, 30, "地址:");
    oAIn[5] = new Fl_Input(85, 270, 150, 30, "手机号:");
    oAIn[6] = new Fl_Input(85, 320, 150, 30, "确认手机号:");
    oAIn[7] = new Fl_Input(85, 370, 150, 30, "*存入金额:");
    for (int i = 0; i < 8; i++) windows[2]->add(oAIn[i]);

    /*销户模块*/
    box[0] = new Fl_Box(110,70,80,45,"正在销户，请输入信息：");
    cAIn[0] = new Fl_Input(85, 180, 150, 30, "账号:");
    cAIn[1] = new Fl_Input(85, 280, 150, 30, "密码:");
    windows[3]->add(box[0]);
    for (int i = 0; i < 2; i++) windows[3]->add(cAIn[i]);

    /*存款模块*/
    box[1] = new Fl_Box(110, 70, 80, 45, "正在存款，请输入信息：");
    sIn[0] = new Fl_Input(85, 180, 150, 30, "账号:");
    sIn[1] = new Fl_Input(85, 280, 150, 30, "存款金额:");
    windows[4]->add(box[1]);
    for (int i = 0; i < 2; i++) windows[4]->add(sIn[i]);

    /*取款模块*/
    box[2] = new Fl_Box(110, 70, 80, 45, "正在取款，请输入信息：");
    wIn[0] = new Fl_Input(85, 180, 150, 30, "账号:");
    wIn[1] = new Fl_Input(85, 230, 150, 30, "密码:");
    wIn[2] = new Fl_Input(85, 280, 150, 30, "取款金额:");
    windows[5]->add(box[2]);
    for (int i = 0; i < 3; i++) windows[5]->add(wIn[i]);

    /*查询模块*/
    box[3] = new Fl_Box(110, 70, 80, 45, "正在查询，请输入信息：");
    srIn[0] = new Fl_Input(85, 180, 150, 30, "账号:");
    srIn[1] = new Fl_Input(85, 280, 150, 30, "密码:");
    windows[6]->add(box[3]);
    for (int i = 0; i < 2; i++) windows[6]->add(srIn[i]);

    /*转账模块*/
    box[4] = new Fl_Box(110, 70, 80, 45, "正在转账，请输入信息：");
    tIn[0] = new Fl_Input(85, 150, 150, 30, "转出账号:");
    tIn[1] = new Fl_Input(85, 190, 150, 30, "密码:");
    tIn[3] = new Fl_Input(85, 230, 150, 30, "转入账号:");
    tIn[2] = new Fl_Input(85, 270, 150, 30, "取款金额:");
    windows[7]->add(box[4]);
    for (int i = 0; i < 4; i++) windows[7]->add(tIn[i]);

    /*绑定手机模块*/
    box[5] = new Fl_Box(110, 70, 80, 45, "正在绑定手机，请输入信息：");
    bIn[0] = new Fl_Input(85, 150, 150, 30, "账号:");
    bIn[1] = new Fl_Input(85, 190, 150, 30, "密码:");
    bIn[2] = new Fl_Input(85, 230, 150, 30, "手机号:");
    bIn[3] = new Fl_Input(85, 270, 150, 30, "确认手机号:");
    windows[8]->add(box[5]);
    for (int i = 0; i < 4; i++) windows[8]->add(bIn[i]);

    /*改密模块*/
    //加一个新的忘记密码按钮
    okBtn[7]->resize(40, 415, 60, 25);
    backBtn[7]->resize(200, 415, 60, 25);
    forgetBtn = new Fl_Button(120, 415, 60, 25,"&忘记密码");
    forgetBtn->callback((Fl_Callback*)Btn10open_cb, fBuf);
    windows[9]->add(forgetBtn);

    box[6] = new Fl_Box(110, 70, 80, 45, "正在更改密码，请输入信息：");
    uIn[0] = new Fl_Input(85, 150, 150, 30, "账号:");
    uIn[1] = new Fl_Input(85, 190, 150, 30, "旧密码:");
    uIn[2] = new Fl_Input(85, 230, 150, 30, "新密码:");
    uIn[3] = new Fl_Input(85, 270, 150, 30, "确认新密码:");
    windows[9]->add(box[6]);
    for (int i = 0; i < 4; i++) windows[9]->add(uIn[i]);

    /*改密中的验证手机模块*/
    box[7] = new Fl_Box(110, 70, 80, 45, "使用已绑定的手机号更改密码，请输入信息：");
    uIn[4] = new Fl_Input(85, 150, 150, 30, "账号:");
    uIn[5] = new Fl_Input(85, 190, 150, 30, "手机号:");
    uIn[6] = new Fl_Input(85, 230, 150, 30, "新密码:");
    uIn[7] = new Fl_Input(85, 270, 150, 30, "确认新密码:");
    windows[10]->add(box[7]);
    for (int i = 4; i < 8; i++) windows[10]->add(uIn[i]);
    Fl::run();
}
/**
功能：开户
说明：在进入系统时。在读入数据过程中，已经记录了用户数为N。在数组中相应下标为0~N-1
  开户时要添加一个用户，仅仅要为下标为N的数组元素置值。并在成功之后令N++就可以。
  账号由系统自己主动生成(10001+序号)。保证其连续。这样在顺序添加的时候。就保证了其有序。

**/
int Bank::openAccount(const char *iPass1,const char *iPass2,const char *id,
    const char *nam,const char *addr,const char *phone1,const char *phone2,const char * bal) {
    if (N == upNum) {
        cout << "银行用户数已经达到上限。不能再开户\n";
        return false;
    }
    ///以下正常办理开户业务
    while (used[newAcc]) newAcc++;
    int sta=0;   ///状态
    int acc = newAcc;///账号
    used[newAcc] = 1;
    const int pasNum = 6;
    int pw1 = checkNum(iPass1,pasNum);
    if (pw1 == -1) return 1;//1:密码输入错误，应为6位且非全0数字
    int pw2 = checkNum(iPass2,pasNum);
    if (pw1 != pw2) return 2;//2:两次密码不一样
    if (strlen(id) != 18) return 3;//3:身份证为空或输入不为18位
    long long ph1 = 0, ph2 = -2;
    if (strlen(phone1) != 0) {
        if (phone1[0] != '1') return 4;//4:手机号应以1开头
        const int pNum = 11;
        ph1 = checkNum(phone1, pNum);
        ph2 = checkNum(phone2, pNum);
        if (ph1 <= 0) return 5;//5:手机号应为11位数字
        if (ph1 != ph2) return 6;//6:两次手机号不一样
    }
    double bals = atof(bal);
    if (bals <= 0) return 7;//7:存款不能为0，或输入错误
    Note p;///构造user对象
    p.setUser(acc, pw1, id, nam, addr, bals, sta, ph1);
    //存钱记录
    p.date = getTime();
    p.add_save_record(bals);
    add(p);///添加链表
    return 0;//成功开户
}
/**
功能：注销账户
**/
int Bank::cancelAccount(const char* account,const char*pas) {
    int acc = checkNum(account,-1);//是否全为数字
    if (acc == -1) return 1;//1:输入的账号错误
    Note* who = getUser(acc);  ///依据账号查询用户，返回指向储存用户信息链表的指针。
    if (who == NULL) return 2;//2:账号不存在
    int pw = checkNum(pas, 6);
    if (pw == -1) return 3;//3:输入密码不合法，应为6位且非全0数字
    if (pw != who->password) return 4;//4:输入密码错误
    char okCancel[260] = "";
    sprintf_s(okCancel, "销户成功！本次取款金额为:%f元", who->balance);
    fl_alert(okCancel);
    who->date = getTime();
    who->add_withdraw_record(who->balance);
    who->balance = 0;  ///取款后剩余金额变0
    who->status = 1;  ///状态变为注销
    return 0;//销户成功
}

/**
功能：存款
说明：须要保证账户存在，且处于正常状态
**/
int Bank::save(const char* account,const char* bal){
    int acc = checkNum(account, -1);//是否全为数字
    if (acc == -1) return 1;//1:输入的账号错误
    Note* who = getUser(acc);  ///依据账号查询用户，返回指向储存用户信息链表的指针。
    if (who == NULL) return 2;//2:账号不存在
    if (who->status == 1) return 3;//3:账号已销户，无法存款
    double bals=atof(bal);
    if (bals <= 0) return 4;//4:输入余额错误
    who->balance += bals;
    //存钱记录
    who->date = getTime();
    who->add_save_record(bals);
    char okBuff[260] = "";
    sprintf_s(okBuff, "存款成功！存款后金额为:%f元", who->balance);
    fl_alert(okBuff);
    return 0;
}

/**
功能：取款
说明：须要保证账户存在。且处于正常状态。另外，剩余金额要足够取
**/
int Bank::withdraw(const char* account,const char* pas,const char*bal)
{
    int acc = checkNum(account, -1);//是否全为数字
    if (acc == -1) return 1;//1:输入的账号错误
    Note* who = getUser(acc);  ///依据账号查询用户，返回指向储存用户信息链表的指针。
    if (who == NULL) return 2;//2:账号不存在
    if (who->status == 1) return 3;//3:账号已销户，无法取款
    int pw = checkNum(pas, 6);
    if (pw == -1) return 4;//4:输入密码不合法，应为6位且非全0数字
    if (pw != who->password) return 5;//5:输入密码错误
    double bals = atof(bal);
    if (bals <= 0) return 6;//6:输入余额错误
    if (bals > who->balance) return 7;//7:取款失败，余额不足
    who->balance -= bals;
    //取钱记录
    who->date = getTime();
    who->add_withdraw_record(bals);
    char okBuff[260] = "";
    sprintf_s(okBuff, "取款成功！取款后金额为:%f元", who->balance);
    fl_alert(okBuff);
    return 0;
}
/**
功能：查询账户
说明：显示账户信息
**/
int Bank::showAccount(const char* account, const char* pas)
{
    int acc = checkNum(account, -1);//是否全为数字
    if (acc == -1) return 1;//1:输入的账号错误
    Note* who = getUser(acc);  ///依据账号查询用户，返回指向储存用户信息链表的指针。
    if (who == NULL) return 2;//2:账号不存在
    if (who->status == 1) return 3;//3:账号已销户，无法取款
    int pw = checkNum(pas, 6);
    if (pw == -1) return 4;//4:输入密码不合法，应为6位且非全0数字
    if (pw != who->password) return 5;//5:输入密码错误
    char okBuff[260] = "";
    sprintf_s(okBuff, "余额为:%f元", who->balance);
    fl_alert(okBuff);
    return 0;
}
/**
功能：转账
说明：须要保证两个账户都存在，且处于正常状态，另外，转出账户的剩余金额要足够
**/
int Bank::transferAccounts(const char* account, const char* pas, const char* bal, const char* account2) {
    int acc = checkNum(account, -1);//是否全为数字
    if (acc == -1) return 1;//1:输入的转出账号错误
    Note* who = getUser(acc);  ///依据账号查询用户，返回指向储存用户信息链表的指针。
    if (who == NULL) return 2;//2:转出账号不存在
    if (who->status == 1) return 3;//3:转出账号已销户，无法转出

    int pw = checkNum(pas, 6);
    if (pw == -1) return 4;//4:输入密码不合法，应为6位且非全0数字
    if (pw != who->password) return 5;//5:输入密码错误

    int acc2 = checkNum(account2, -1);
    if (acc2 == -1) return 6;//6:输入的转入账号错误
    Note* who2 = getUser(acc2); 
    if (who2 == NULL) return 7;//7:转入账号不存在
    if (who2->status == 1) return 8;//8:转入账号已销户，无法接受

    double bals = atof(bal);
    if (bals <= 0) return 9;//9:输入余额错误
    if (bals > who->balance) return 10;//10:转账失败，余额不足

    who->balance -= bals;
    who2->balance += bals;
    //取钱记录
    who->date = getTime();
    who->add_withdraw_record(bals);
    //存钱记录
    who2->date = getTime();
    who2->add_save_record(bals);
    char okBuff[260] = "";
    sprintf_s(okBuff, "转账成功！转账金额为%f元,转账后余额为:%f元", bals, who->balance);
    fl_alert(okBuff);
    return 0;
}
/**
功能：绑定手机号
说明：手机号用于忘记password时候改动password用
**/
int Bank::bindingPhonenumber(const char* account, const char* pas, const char* phone1, const char* phone2) {
    int acc = checkNum(account, -1);//是否全为数字
    if (acc == -1) return 1;//1:输入的账号错误
    Note* who = getUser(acc);  ///依据账号查询用户，返回指向储存用户信息链表的指针。
    if (who == NULL) return 2;//2:账号不存在
    if (who->status == 1) return 3;//3:账号已销户，无法绑定
    int pw = checkNum(pas, 6);
    if (pw == -1) return 4;//4:输入密码不合法，应为6位且非全0数字
    if (pw != who->password) return 5;//5:输入密码错误
    long long ph1 = 0, ph2 = -2;
    if (strlen(phone1) != 0) {
        if (phone1[0] != '1') return 6;//4:手机号应以1开头
        const int pNum = 11;
        ph1 = checkNum(phone1, pNum);
        ph2 = checkNum(phone2, pNum);
        if (ph1 <= 0) return 7;//5:手机号应为11位数字
        if (ph1 != ph2) return 8;//6:两次手机号不一样
    }
    who->phoneNumber = ph1;
    char okBuff[260] = "";
    sprintf_s(okBuff, "绑定成功!");
    fl_alert(okBuff);
    return 0;
}
/**
功能：改密
**/
int Bank::updatePassword(const char* account, const char* oldPas, const char* newPas1, const char* newPas2) {
    int acc = checkNum(account, -1);//是否全为数字
    if (acc == -1) return 1;//1:输入的账号错误
    Note* who = getUser(acc);  ///依据账号查询用户，返回指向储存用户信息链表的指针。
    if (who == NULL) return 2;//2:账号不存在
    if (who->status == 1) return 3;//3:账号已销户，无法改密
    int pw = checkNum(oldPas, 6);
    if (pw == -1) return 4;//4:输入的旧密码不合法，应为6位且非全0数字
    if (pw != who->password) return 5;//5:输入旧密码错误
    int pw1 = checkNum(newPas1, 6);
    if (pw1 == -1) return 6;//6:新密码输入错误，应为6位且非全0数字
    int pw2 = checkNum(newPas2, 6);
    if (pw1 != pw2) return 7;//7:两次新密码不一样
    who->password = pw1;
    char okBuff[260] = "";
    sprintf_s(okBuff, "改密成功!");
    fl_alert(okBuff);
    return 0;
}
/**
功能：改密-手机版
**/
int Bank::updatePassword_phone(const char* account, const char* phone, const char* newPas1, const char* newPas2) {
    int acc = checkNum(account, -1);//是否全为数字
    if (acc == -1) return 1;//1:输入的账号错误
    Note* who = getUser(acc);  ///依据账号查询用户，返回指向储存用户信息链表的指针。
    if (who == NULL) return 2;//2:账号不存在
    if (who->status == 1) return 3;//3:账号已销户，无法改密

    long long ph = 0;
    if (strlen(phone) == 0) return 4;//4:手机号不能为空
    if (phone[0] != '1') return 5;//5:手机号应以1开头
    ph = checkNum(phone, 11);
    if (ph <= 0) return 6;//6:手机号应为11位数字
    if (ph != who->phoneNumber) return 7;//7:输入手机号错误

    int pw1 = checkNum(newPas1, 6);
    if (pw1 == -1) return 8;//8:新密码输入错误，应为6位且非全0数字
    int pw2 = checkNum(newPas2, 6);
    if (pw1 != pw2) return 9;//9:两次新密码不一样
    who->password = pw1;
    char okBuff[260] = "";
    sprintf_s(okBuff, "改密成功!");
    fl_alert(okBuff);
    return 0;
}
/**
功能：添加链表成员，添加用户。
**/
void Bank::add(User a) {
    Note* p, * s;
    s = (Note*)new(Note); ///动态分配一个新结点
    s->noteSet(a); ///设a为此结点
    p = head;
    ///若是空表，使a作为第一个结点
    if (head == NULL) {
        head = s;
        s->next = NULL;
    }
    else {
        ///插入末尾
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = s;
        s->next = NULL;
        
    }
}
/**
功能：输入账号查询用户 ，历遍链表找到账号
入口參数：要查询用户的账号
返回值：假设该用户存在，返回指向该用户链表的指针
**/
Note* Bank::getUser(int acc) {
    Note* f = head;
    ///历遍链表查找账号
    while (f != NULL) {
        if (f->account == acc) return f;
        f = f->next;
    }
    return f;
}
/*检查密码是否是w位数字且非0*/
long long checkNum(const char* pas,const int w) {
    if (w != -1) {//不要求位数
        if (strlen(pas) != w) return -1;
    }
    long long pass = 0;
    for (int i = 0; pas[i] != '\0'; i++) {
        if (!isdigit(pas[i])) return -1;
        pass = pass * 10 + pas[i] - '0';
    }
    if (pass == 0) return -1;
    return pass;
}
/*返回现在开户的账号*/
int Bank::nowAcc() {
    return newAcc;
}
