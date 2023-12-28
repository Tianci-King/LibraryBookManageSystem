#ifndef _INCLUDE_USER_H_
#define _INCLUDE_USER_H_

#include <iostream>
using namespace std;

class Admin {
private:
    string name;      // 图书管理员名
    string password;  // 密码

public:
    Admin();
    Admin(const string &name, const string &password);

    ~Admin();

    const string &GetName() const;
    void SetName(const string &name);

    const string &GetPassword() const;
    void SetPassword(const string &password);

    friend istream &operator>>(istream &in, Admin &user);
    friend ostream &operator<<(ostream &os, const Admin &user);
};

#endif  // _INCLUDE_USER_H_