#ifndef _INCLUDE_BOOK_H_
#define _INCLUDE_BOOK_H_

#include <iostream>
using namespace std;

class Book {
private:
    int id;             // 编号
    string ISBN;        // 国际标准书号
    string name;        // 书名
    string author;      // 作者
    string publisher;   // 出版社
    int year;           // 出版年份
    bool borrowStatus;  // 借阅状态 0: 在库中, 1: 借阅中
    string borrower;    // 借阅人

public:
    Book();
    Book(int id, const string& ISBN, const string& name, const string& author,
         const string& publisher, int year, bool borrowStatus,
         const string& borrower);

    ~Book();

    const int& GetId() const;
    void SetId(int id);

    const string& GetISBN() const;
    void SetISBN(const string& isbn);

    const string& GetName() const;
    void SetName(const string& name);

    const string& GetAuthor() const;
    void SetAuthor(const string& author);

    const string& GetPublisher() const;
    void SetPublisher(const string& publisher);

    int GetYear() const;
    void SetYear(int year);

    bool GetBorrowStatus() const;
    void SetBorrowStatus(bool borrow_status);

    const string& GetBorrower() const;
    void SetBorrower(const string& borrower);

    friend istream& operator>>(istream& in, Book& book);
    friend ostream& operator<<(ostream& out, const Book& book);
};

#endif  // _INCLUDE_BOOK_H_