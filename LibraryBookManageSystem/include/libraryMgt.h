#ifndef _INCLUDE_BOOKMANAGER_H_
#define _INCLUDE_BOOKMANAGER_H_

#include "book.h"
#include "rbTree.h"
#include "FileOperation.h"

class LibraryManager {
private:
    struct IdOfBook {
        const int& operator()(const Book& b) const { return b.GetId(); }
    };
    typedef RbTree<int, Book, IdOfBook, std::less<>> RbTree;

    RbTree libraryManager;

public:
    LibraryManager();

    ~LibraryManager();

    // 初始化书籍数据
    void Init(string path, string fileType);

    // 添加书籍
    void Insert();

    // 当前书籍是否为空
    bool Empty();

    // 分页查询所有书籍
    void FindByPage(int currPage, int pageSize);

    // 根据书籍编号查找书籍
    void FindByID();

    // 根据ISBN号查找书籍
    void FindByISBN();

    // 根据书籍编号更新书籍信息
    void UpdateByID();

    // 根据ISBN号更新书籍信息
    void UpdateByISBN();

    // 根据书籍编号删除书籍
    void RemoveByID();

    // 根据ISBN号删除书籍
    void RemoveByISBN();

    // 借出书籍
    void Lend();

    // 归还书籍
    void Return();

    // 查看所有已借出的书籍
    void FindAllLend();

    // 保存数据
    void Save(string path, string fileType);

    friend class FileOperation;
};

#endif  // _INCLUDE_BOOKMANAGER_H_