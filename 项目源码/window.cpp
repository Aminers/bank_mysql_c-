#include "window.h"
#include "bank.h"
Fl_Window* windows[11];
Fl_Button* okBtn[10], * goBtn[8], * backBtn[9] ,* forgetBtn;
extern Fl_Input* sNames, * sPasss, * oAIn[8], * cAIn[2], * sIn[2], * wIn[3], * srIn[2], * tIn[4],*uIn[8],*bIn[4];
//切换界面按钮
void Btn0login_cb(Fl_Widget* w, void* data) {
	((Fl_Button*)w)->label((char*)data);
	if (pass(sNames->value(), sPasss->value())) {//密码正确，进入选择窗口。
		windows[0]->hide(); windows[1]->show();
	}
	else {
		fl_alert("账号或密码错误，请重新输入!");
	}
}
//2.开户按钮
void Btn2open_cb(Fl_Widget* w, void* data) {
	((Fl_Button*)w)->label((char*)data);
	windows[1]->hide(); windows[2]->show();
}
void Btn2close_cb(Fl_Widget* w, void* data) {
	((Fl_Button*)w)->label((char*)data);
	windows[2]->hide(); windows[1]->show();
}
void Btn2ok_cb(Fl_Widget* w, void* data) {
	((Fl_Button*)w)->label((char*)data);
	//返回类型：
	//0:成功
	//1:密码输入错误，应为6位且非全0数字
	//2:两次密码不一样
	//3:身份证为空或输入不为18位
	//4:手机号应以1开头
	//5:手机号应为11位数字
	//6:两次手机号不一样
	//7:存款不能为0，或输入错误
	switch (Bank::openAccount(oAIn[0]->value(), oAIn[1]->value(), oAIn[2]->value(), oAIn[3]->value(),
		oAIn[4]->value(), oAIn[5]->value(), oAIn[6]->value(), oAIn[7]->value()))
	{
	case 0: {
		char okOpen[260] = "";
		sprintf_s(okOpen, "开户成功,你的账号为%d", Bank::nowAcc());
		fl_alert(okOpen); 
		windows[2]->hide(); windows[1]->show(); break;
	}
	case 1: {fl_alert("密码输入错误，应为6位且非全0数字"); break; }
	case 2: {fl_alert("两次密码不一样"); break; }
	case 3: {fl_alert("身份证为空或输入不为18位"); break; }
	case 4: {fl_alert("手机号应以1开头"); break; }
	case 5: {fl_alert("手机号应为11位数字"); break; }
	case 6: {fl_alert("两次手机号不一样"); break; }
	case 7: {fl_alert("存款不能为0，或输入错误"); break; }
	default:
		break;
	}
}
//3.销户
void Btn3open_cb(Fl_Widget* w, void* data) {
	((Fl_Button*)w)->label((char*)data);
	windows[1]->hide(); windows[3]->show();
}
void Btn3close_cb(Fl_Widget* w, void* data) {
	((Fl_Button*)w)->label((char*)data);
	windows[3]->hide(); windows[1]->show();
}
void Btn3ok_cb(Fl_Widget* w, void* data) {
	((Fl_Button*)w)->label((char*)data);
	//0:成功
	//1:输入的账号错误
	//2:账号不存在
	//3:输入密码不合法，应为6位且非全0数字
	//4:输入密码错误
	switch (Bank::cancelAccount(cAIn[0]->value(),cAIn[1]->value()))
	{
	case 0: {
		windows[3]->hide(); windows[1]->show(); break;
	}
	case 1: {fl_alert("输入的账号错误"); break; }
	case 2: {fl_alert("账号不存在"); break; }
	case 3: {fl_alert("输入密码不合法，应为6位且非全0数字"); break; }
	case 4: {fl_alert("输入密码错误"); break; }
	default:
		break;
	}
}
//4.存款
void Btn4open_cb(Fl_Widget* w, void* data) {
	((Fl_Button*)w)->label((char*)data);
	windows[1]->hide(); windows[4]->show();
}
void Btn4close_cb(Fl_Widget* w, void* data) {
	((Fl_Button*)w)->label((char*)data);
	windows[4]->hide(); windows[1]->show();
}
void Btn4ok_cb(Fl_Widget* w, void* data) {
	((Fl_Button*)w)->label((char*)data);
	//0:成功
	//1:输入的账号错误
	//2:账号不存在
	//3:账号已销户，无法存款
	//4:输入余额错误
	switch (Bank::save(sIn[0]->value(), sIn[1]->value()))
	{
	case 0: {
		windows[4]->hide(); windows[1]->show(); break;
	}
	case 1: {fl_alert("输入的账号错误"); break; }
	case 2: {fl_alert("账号不存在"); break; }
	case 3: {fl_alert("账号已销户，无法存款"); break; }
	case 4: {fl_alert("输入余额错误"); break; }
	default:
		break;
	}
}
//5.取款
void Btn5open_cb(Fl_Widget* w, void* data) {
	((Fl_Button*)w)->label((char*)data);
	windows[1]->hide(); windows[5]->show();
}
void Btn5close_cb(Fl_Widget* w, void* data) {
	((Fl_Button*)w)->label((char*)data);
	windows[5]->hide(); windows[1]->show();
}
void Btn5ok_cb(Fl_Widget* w, void* data) {
	((Fl_Button*)w)->label((char*)data);
	//0:成功
	//1:输入的账号错误
	//2:账号不存在
	//3:账号已销户，无法取款
	//4:输入密码不合法，应为6位且非全0数字
	//5:输入密码错误
	//6:输入余额错误
	//7:取款失败，余额不足
	switch (Bank::withdraw(wIn[0]->value(), wIn[1]->value(), wIn[2]->value()))
	{
	case 0: {
		windows[5]->hide(); windows[1]->show(); break;
	}
	case 1: {fl_alert("输入的账号错误"); break; }
	case 2: {fl_alert("账号不存在"); break; }
	case 3: {fl_alert("账号已销户，无法取款"); break; }
	case 4: {fl_alert("输入密码不合法，应为6位且非全0数字"); break; }
	case 5: {fl_alert("输入密码错误"); break; }
	case 6: {fl_alert("输入余额错误"); break; }
	case 7: {fl_alert("取款失败，余额不足"); break; }
	default:
		break;
	}
}
//6.查询
void Btn6open_cb(Fl_Widget* w, void* data) {
	((Fl_Button*)w)->label((char*)data);
	windows[1]->hide(); windows[6]->show();
}
void Btn6close_cb(Fl_Widget* w, void* data) {
	((Fl_Button*)w)->label((char*)data);
	windows[6]->hide(); windows[1]->show();
}
void Btn6ok_cb(Fl_Widget* w, void* data) {
	((Fl_Button*)w)->label((char*)data);
	//0:成功
	//1:输入的账号错误
	//2:账号不存在
	//3:账号已销户，无法取款
	//4:输入密码不合法，应为6位且非全0数字
	//5:输入密码错误
	switch (Bank::showAccount(srIn[0]->value(), srIn[1]->value()))
	{
	case 0: {
		windows[6]->hide(); windows[1]->show(); break;
	}
	case 1: {fl_alert("输入的账号错误"); break; }
	case 2: {fl_alert("账号不存在"); break; }
	case 3: {fl_alert("账号已销户，无法取款"); break; }
	case 4: {fl_alert("输入密码不合法，应为6位且非全0数字"); break; }
	case 5: {fl_alert("输入密码错误"); break; }
	default:
		break;
	}
}
//7.转账
void Btn7open_cb(Fl_Widget* w, void* data) {
	((Fl_Button*)w)->label((char*)data);
	windows[1]->hide(); windows[7]->show();
}
void Btn7close_cb(Fl_Widget* w, void* data) {
	((Fl_Button*)w)->label((char*)data);
	windows[7]->hide(); windows[1]->show();
}
void Btn7ok_cb(Fl_Widget* w, void* data) {
	((Fl_Button*)w)->label((char*)data);
	//0:成功
	//1:输入的转出账号错误
	//2:转出账号不存在
	//3:转出账号已销户，无法转出
	//4:输入密码不合法，应为6位且非全0数字
	//5:输入密码错误
	//6:输入的转入账号错误
	//7:转入账号不存在
	//8:转入账号已销户，无法接受
	//9:输入余额错误
	//10:转账失败，余额不足
	switch (Bank::transferAccounts(tIn[0]->value(), tIn[1]->value(), tIn[2]->value(),tIn[3]->value()))
	{
	case 0: {
		windows[7]->hide(); windows[1]->show(); break;
	}
	case 1: {fl_alert("输入的转出账号错误"); break; }
	case 2: {fl_alert("转出账号不存在"); break; }
	case 3: {fl_alert("转出账号已销户，无法转出"); break; }
	case 4: {fl_alert("输入密码不合法，应为6位且非全0数字"); break; }
	case 5: {fl_alert("输入密码错误"); break; }
	case 6: {fl_alert("输入的转入账号错误"); break; }
	case 7: {fl_alert("转入账号不存在"); break; }
	case 8: {fl_alert("转入账号已销户，无法接受"); break; }
	case 9: {fl_alert("输入余额错误"); break; }
	case 10: {fl_alert("转账失败，余额不足"); break; }
	default:
		break;
	}
}
//8.绑定
void Btn8open_cb(Fl_Widget* w, void* data) {
	((Fl_Button*)w)->label((char*)data);
	windows[1]->hide(); windows[8]->show();
}
void Btn8close_cb(Fl_Widget* w, void* data) {
	((Fl_Button*)w)->label((char*)data);
	windows[8]->hide(); windows[1]->show();
}
void Btn8ok_cb(Fl_Widget* w, void* data) {
	((Fl_Button*)w)->label((char*)data);
	//0:成功
	//1:输入的账号错误
	//2:账号不存在
	//3:账号已销户，无法绑定
	//4:输入密码不合法，应为6位且非全0数字
	//5:输入密码错误
	//6:手机号应以1开头
	//7:手机号应为11位数字
	//8:两次手机号不一样
	switch (Bank::bindingPhonenumber(bIn[0]->value(), bIn[1]->value(), bIn[2]->value(), bIn[3]->value()))
	{
	case 0: {
		windows[8]->hide(); windows[1]->show(); break;
	}
	case 1: {fl_alert("输入的账号错误"); break; }
	case 2: {fl_alert("账号不存在"); break; }
	case 3: {fl_alert("账号已销户，无法绑定"); break; }
	case 4: {fl_alert("输入密码不合法，应为6位且非全0数字"); break; }
	case 5: {fl_alert("输入密码错误"); break; }
	case 6: {fl_alert("手机号应以1开头"); break; }
	case 7: {fl_alert("手机号应为11位数字"); break; }
	case 8: {fl_alert("两次手机号不一样"); break; }
	default:
		break;
	}
}
//9.改密
void Btn9open_cb(Fl_Widget* w, void* data) {
	((Fl_Button*)w)->label((char*)data);
	windows[1]->hide(); windows[9]->show();
}
void Btn9close_cb(Fl_Widget* w, void* data) {
	((Fl_Button*)w)->label((char*)data);
	windows[9]->hide(); windows[1]->show();
}
void Btn9ok_cb(Fl_Widget* w, void* data) {
	((Fl_Button*)w)->label((char*)data);
	//0:成功
	//1:输入的账号错误
	//2:账号不存在
	//3:账号已销户，无法改密
	//4:输入的旧密码不合法，应为6位且非全0数字
	//5:输入旧密码错误
	//6:新密码输入错误，应为6位且非全0数字
	//7:两次新密码不一样
	switch (Bank::updatePassword(uIn[0]->value(), uIn[1]->value(), uIn[2]->value(), uIn[3]->value()))
	{
	case 0: {
		windows[9]->hide(); windows[1]->show(); break;
	}
	case 1: {fl_alert("输入的账号错误"); break; }
	case 2: {fl_alert("账号不存在"); break; }
	case 3: {fl_alert("账号已销户，无法改密"); break; }
	case 4: {fl_alert("输入的旧密码不合法，应为6位且非全0数字"); break; }
	case 5: {fl_alert("输入旧密码错误"); break; }
	case 6: {fl_alert("新密码输入错误，应为6位且非全0数字"); break; }
	case 7: {fl_alert("两次新密码不一样"); break; }
	default:
		break;
	}
}
//9.2改密时忘记密码的按钮
void Btn10open_cb(Fl_Widget* w, void* data) {
	((Fl_Button*)w)->label((char*)data);
	windows[9]->hide(); windows[10]->show();
}
void Btn10close_cb(Fl_Widget* w, void* data) {
	((Fl_Button*)w)->label((char*)data);
	windows[10]->hide(); windows[9]->show();
}
void Btn10ok_cb(Fl_Widget* w, void* data) {
	((Fl_Button*)w)->label((char*)data);
	//0:成功
	//1:输入的账号错误
	//2:账号不存在
	//3:账号已销户，无法改密
	//4:手机号不能为空
	//5:手机号应以1开头
	//6:手机号应为11位数字
	//7:输入手机号错误
	//8:新密码输入错误，应为6位且非全0数字
	//9:两次新密码不一样
	switch (Bank::updatePassword_phone(uIn[4]->value(), uIn[5]->value(), uIn[6]->value(), uIn[7]->value()))
	{
	case 0: {
		windows[10]->hide(); windows[1]->show(); break;
	}
	case 1: {fl_alert("输入的账号错误"); break; }
	case 2: {fl_alert("账号不存在"); break; }
	case 3: {fl_alert("账号已销户，无法改密"); break; }
	case 4: {fl_alert("手机号不能为空"); break; }
	case 5: {fl_alert("手机号应以1开头"); break; }
	case 6: {fl_alert("手机号应为11位数字"); break; }
	case 7: {fl_alert("输入手机号错误"); break; }
	case 8: {fl_alert("新密码输入错误，应为6位且非全0数字"); break; }
	case 9: {fl_alert("两次新密码不一样"); break; }
	default:
		break;
	}
}
