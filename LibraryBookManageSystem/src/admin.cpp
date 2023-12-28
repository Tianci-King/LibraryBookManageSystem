#include "admin.h"

Admin::Admin() = default;
Admin::Admin(const string &name, const string &password)
    : name(name), password(password) {}

Admin::~Admin() = default;

const string &Admin::GetName() const { return name; }
void Admin::SetName(const string &name) { Admin::name = name; }

const string &Admin::GetPassword() const { return password; }
void Admin::SetPassword(const string &password) { Admin::password = password; }

istream &operator>>(istream &in, Admin &user) {
    in >> user.name >> user.password;
    return in;
}
ostream &operator<<(ostream &out, const Admin &user) {
    out << user.name << " " << user.password;
    return out;
}
