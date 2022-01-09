/*==============================================================*/
/* DBMS name:      MySQL 5.0                                    */
/* Created on:     2022/1/6 12:18:04                            */
/*==============================================================*/

drop table if exists record_account;

drop table if exists account_id;

drop table if exists account;

drop table if exists admin_login;

drop table if exists record;

drop table if exists user;



/*==============================================================*/
/* Table: account                                               */
/*==============================================================*/
create table account
(
   acc                  int not null auto_increment,
   passwd               int not null,
   state                bool not null,
   balance              float not null,
   primary key (acc)
);

/*==============================================================*/
/* Table: account_record                                        */
/*==============================================================*/
create table account_id
(
   acc                  int not null,
   id                   char(18) not null,
   primary key (acc)
);

/*==============================================================*/
/* Table: admin_login                                           */
/*==============================================================*/
create table admin_login
(
   wname                char(5) not null,
   wpasswd              varchar(20) not null,
   primary key (wname)
);

/*==============================================================*/
/* Table: record                                                */
/*==============================================================*/
create table record
(
   rno                  int not null auto_increment,
   rdate                date not null,
   money                float not null,
   opt                boolean not null,
   primary key (rno)
);

/*==============================================================*/
/* Table: record_account                                        */
/*==============================================================*/
create table record_account
(
   rno                  int not null auto_increment,
   acc                  int not null,
   primary key (rno)
);

/*==============================================================*/
/* Table: user                                                  */
/*==============================================================*/
create table user
(
   id                   char(18) not null,
   name                 varchar(10),
   addr                 varchar(40),
   phone                bigint,
   primary key (id)
);


/*默认添加管理员用户名admin,密码123456*/

insert into admin_login
values("admin","123456");