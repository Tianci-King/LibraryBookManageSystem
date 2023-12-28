#ifndef _INCLUDE_BOOK_H_
#define _INCLUDE_BOOK_H_

#include <iostream>
using namespace std;

class Book {
private:
    int id;             // ���
    string ISBN;        // ���ʱ�׼���
    string name;        // ����
    string author;      // ����
    string publisher;   // ������
    int year;           // �������
    bool borrowStatus;  // ����״̬ 0: �ڿ���, 1: ������
    string borrower;    // ������

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