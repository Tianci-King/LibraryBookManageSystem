#ifndef _INCLUDE_USERMANAGER_H_
#define _INCLUDE_USERMANAGER_H_

#include "rbTree.h"
#include "admin.h"
#include "FileOperation.h"

class AdminManager {
private:
    struct NameOfUser {
        const string& operator()(const Admin& u) const { return u.GetName(); }
    };
    typedef RbTree<string, Admin, NameOfUser, std::less<>> RbTree;

    RbTree adminManager;

public:
    AdminManager();

    ~AdminManager();

    // 初始化用户数据
    void Init(string path, string fileType);

    // 注册
    bool Register();

    // 登录
    bool Login();

    // 保存数据
    void Save(string path, string fileType);

    friend class FileOperation;
};

#endif  // _INCLUDE_USERMANAGER_H_