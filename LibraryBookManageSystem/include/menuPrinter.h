#ifndef _INCLUDE_MENU_H_
#define _INCLUDE_MENU_H_

class Menu {
public:
    // 初始菜单
    static void Start();

    // 主菜单
    static void Main();

    // 查找菜单
    static void Find();

    // 更新菜单
    static void Update();

    // 删除菜单
    static void Remove();

    // 借出或归还菜单
    static void LendOrReturn();
};

#endif  // _INCLUDE_MENU_H_