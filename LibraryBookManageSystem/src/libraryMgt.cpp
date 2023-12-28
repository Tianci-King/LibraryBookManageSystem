#include "libraryMgt.h"

#include <fstream>
#include <iostream>
using namespace std;

LibraryManager::LibraryManager() = default;

LibraryManager::~LibraryManager() = default;

void LibraryManager::Init(string path, string fileType) {
    string file = path + fileType;

    ifstream in;
    in.open(file, ios::in);
    if (!in.is_open()) {
        ofstream out;
        out.open(file, ios::out | ios::app);
        out.close();
        in.open(file, ios::in);
    }

    Book book;
    while (in.peek() != EOF) {
        in >> book;
        libraryManager.insertUnique(libraryManager.end(), book);
    }

    in.close();
}

void LibraryManager::Insert() {
    string ISBN, name, author, publisher;
    int year, count;

    cout << "Please enter ISBN number: ";
    cin >> ISBN;
    cout << "Please enter the book name: ";
    cin >> name;
    cout << "Please enter the author: ";
    cin >> author;
    cout << "Please enter the publisher: ";
    cin >> publisher;
    cout << "Please enter the publication year: ";
    cin >> year;
    cout << "Please enter the quantity: ";
    cin >> count;

    cout << "Please confirm the book information (enter y/yes to confirm) : " << endl
         << "ISBN: " << ISBN << "  Book Name: " << name << "  Author: " << author
         << "  Publisher: " << publisher << "  Publication Year: " << year
         << "  Quantity: " << count << "\n> ";
    string confirm;
    cin.get();
    getline(cin, confirm);

    if (confirm == "y" || confirm == "yes") {
        while (count--) {
            libraryManager.insertUnique(libraryManager.end(),
                                        Book(libraryManager.size() + 1, ISBN, name,
                                             author, publisher, year, false, ""));
        }
        cout << "Successfully added" << endl;
    } else {
        cout << "Addition cancelled" << endl;
    }

    cout << "Do you want to continue updating? (enter y/yes to continue)\n> ";
    getline(cin, confirm);
    if (confirm == "y" || confirm == "yes") {
        Insert();
    }
}

bool LibraryManager::Empty() { return libraryManager.empty(); }

void LibraryManager::FindByPage(int currPage, int pageSize) {
    size_t size = libraryManager.size();
    size_t totalPages = (size - 1) / pageSize + 1;
    if (currPage < 0 || currPage > totalPages) {
        cout << "Incorrect page number entered, please re-enter (enter 0 to exit)\n> ";
        if (cin >> currPage && currPage) {
            FindByPage(currPage, pageSize);
        }
        return;
    }

    int firstIndex = (currPage - 1) * pageSize;
    auto it = libraryManager.begin();
    while (firstIndex--) {
        ++it;
    }
    cout << "-----" << endl;
    int n = pageSize;
    while (n-- && it != libraryManager.end()) {
        cout << *(it++) << endl;
    }
    cout << "-----" << endl;

    cout << "Currently on page " << currPage << " of " << totalPages
         << ", please enter the page number to jump to (enter 0 to exit)\n> ";
    if (cin >> currPage && currPage) {
        FindByPage(currPage, pageSize);
    }
}

void LibraryManager::FindByID() {
    int id;
    cout << "Please enter the book ID to search for: ";
    cin >> id;

    auto it = libraryManager.find(id);
    if (it != libraryManager.end()) {
        cout << *it << endl;
    } else {
        cout << "This book ID does not exist" << endl;
    }

    cout << "Do you want to continue searching? (enter y/yes to continue)\n> ";
    string confirm;
    cin.get();
    getline(cin, confirm);
    if (confirm == "y" || confirm == "yes") {
        FindByID();
    }
}

void LibraryManager::FindByISBN() {
    string ISBN;
    cout << "Please enter the ISBN number to search for: ";
    cin >> ISBN;

    bool find = false;
    int inCount = 0, outCount = 0;
    auto it = libraryManager.begin();
    while (it != libraryManager.end()) {
        if (it->GetISBN() == ISBN) {
            if (!find) {
                cout << "ISBN: " << it->GetISBN()
                     << "  Book Name: " << it->GetName()
                     << "  Author: " << it->GetAuthor()
                     << "  Publisher: " << it->GetPublisher()
                     << "  Publication Year: " << it->GetYear() << endl;
                find = true;
            }
            if (it->GetBorrowStatus()) {
                cout << "Book ID: " << it->GetId()
                     << "  Borrower: " << it->GetBorrower() << endl;
                ++outCount;
            } else {
                ++inCount;
            }
        }
        ++it;
    }
    if (find) {
        cout << "Total " << inCount + outCount << " books  ";
        if (outCount) {
            cout << "Borrowed: " << outCount << " books  ";
        }
        cout << "In Library: " << inCount << " books" << endl;
    } else {
        cout << "This ISBN number does not exist" << endl;
    }

    cout << "Do you want to continue searching? (enter y/yes to continue)\n> ";
    string confirm;
    cin.get();
    getline(cin, confirm);
    if (confirm == "y" || confirm == "yes") {
        FindByISBN();
    }
}

void LibraryManager::UpdateByID() {
    int id;
    cout << "Please enter the book ID to update: ";
    cin >> id;

    auto it = libraryManager.find(id);
    if (it != libraryManager.end()) {
        cout << "Original book information: " << endl
             << "ISBN: " << it->GetISBN() << "  Book Name: " << it->GetName()
             << "  Author: " << it->GetAuthor()
             << "  Publisher: " << it->GetPublisher()
             << "  Publication Year: " << it->GetYear() << endl
             << "Do you want to update? (enter y/yes to update)\n> ";
        string confirm;
        cin.get();
        getline(cin, confirm);
        if (confirm == "y" || confirm == "yes") {
            string updateISBN, updateName, updateAuthor, updatePublisher;
            int updateYear;

            cout << "Please enter the updated ISBN number (enter 0 to not update): ";
            cin >> updateISBN;
            if (updateISBN == "0") {
                updateISBN = it->GetISBN();
            }
            cout << "Please enter the updated book name (enter 0 to not update): ";
            cin >> updateName;
            if (updateName == "0") {
                updateName = it->GetName();
            }
            cout << "Please enter the updated author (enter 0 to not update): ";
            cin >> updateAuthor;
            if (updateAuthor == "0") {
                updateAuthor = it->GetAuthor();
            }
            cout << "Please enter the updated publisher (enter 0 to not update): ";
            cin >> updatePublisher;
            if (updatePublisher == "0") {
                updatePublisher = it->GetPublisher();
            }
            cout << "Please enter the updated publication year (enter 0 to not update): ";
            cin >> updateYear;
            if (updateYear == 0) {
                updateYear = it->GetYear();
            }

            cout << "Please confirm the updated book information (enter y/yes to confirm) : " << endl
                 << "ISBN: " << updateISBN << "  Book Name: " << updateName
                 << "  Author: " << updateAuthor
                 << "  Publisher: " << updatePublisher
                 << "  Publication Year: " << updateYear << "\n> ";
            cin.get();
            getline(cin, confirm);
            if (confirm == "y" || confirm == "yes") {
                it->SetISBN(updateISBN);
                it->SetName(updateName);
                it->SetAuthor(updateAuthor);
                it->SetPublisher(updatePublisher);
                it->SetYear(updateYear);
                cout << "Successfully updated" << endl;
            } else {
                cout << "Update cancelled" << endl;
            }
        } else {
            cout << "Update cancelled" << endl;
        }
    } else {
        cout << "This book ID does not exist" << endl;
        cin.get();
    }

    cout << "Do you want to continue updating? (enter y/yes to continue)\n> ";
    string confirm;
    getline(cin, confirm);
    if (confirm == "y" || confirm == "yes") {
        UpdateByID();
    }
}

void LibraryManager::UpdateByISBN() {
    string ISBN;
    cout << "Please enter the ISBN number of the book to update: ";
    cin >> ISBN;

    bool find = false;
    auto it = libraryManager.begin();
    while (it != libraryManager.end()) {
        if (it->GetISBN() == ISBN) {
            find = true;
            break;
        }
        ++it;
    }
    if (find) {
        cout << "Original book information: " << endl
             << "ISBN: " << it->GetISBN() << "  Book Name: " << it->GetName()
             << "  Author: " << it->GetAuthor()
             << "  Publisher: " << it->GetPublisher()
             << "  Publication Year: " << it->GetYear() << endl
             << "Do you want to update? (enter y/yes to update)\n> ";
        string confirm;
        cin.get();
        getline(cin, confirm);
        if (confirm == "y" || confirm == "yes") {
            string updateISBN, updateName, updateAuthor, updatePublisher;
            int updateYear;

            cout << "Please enter the updated ISBN number (enter 0 to not update): ";
            cin >> updateISBN;
            if (updateISBN == "0") {
                updateISBN = it->GetISBN();
            }
            cout << "Please enter the updated book name (enter 0 to not update): ";
            cin >> updateName;
            if (updateName == "0") {
                updateName = it->GetName();
            }
            cout << "Please enter the updated author (enter 0 to not update): ";
            cin >> updateAuthor;
            if (updateAuthor == "0") {
                updateAuthor = it->GetAuthor();
            }
            cout << "Please enter the updated publisher (enter 0 to not update): ";
            cin >> updatePublisher;
            if (updatePublisher == "0") {
                updatePublisher = it->GetPublisher();
            }
            cout << "Please enter the updated publication year (enter 0 to not update): ";
            cin >> updateYear;
            if (updateYear == 0) {
                updateYear = it->GetYear();
            }

            cout << "Please confirm the updated book information (enter y/yes to confirm) : " << endl
                 << "ISBN: " << updateISBN << "  Book Name: " << updateName
                 << "  Author: " << updateAuthor
                 << "  Publisher: " << updatePublisher
                 << "  Publication Year: " << updateYear << "\n> ";
            cin.get();
            getline(cin, confirm);

            if (confirm == "y" || confirm == "yes") {
                while (it != libraryManager.end()) {
                    if (it->GetISBN() == ISBN) {
                        it->SetISBN(updateISBN);
                        it->SetName(updateName);
                        it->SetAuthor(updateAuthor);
                        it->SetPublisher(updatePublisher);
                        it->SetYear(updateYear);
                    }
                    ++it;
                }
                cout << "Successfully updated" << endl;
            } else {
                cout << "Update cancelled" << endl;
            }
        } else {
            cout << "Update cancelled" << endl;
        }
    } else {
        cout << "This ISBN number does not exist" << endl;
        cin.get();
    }

    cout << "Do you want to continue updating? (enter y/yes to continue)\n> ";
    string confirm;
    getline(cin, confirm);
    if (confirm == "y" || confirm == "yes") {
        UpdateByISBN();
    }
}

void LibraryManager::RemoveByID() {
    int id;
    cout << "Please enter the book ID to delete: ";
    cin >> id;

    auto it = libraryManager.find(id);
    if (it != libraryManager.end()) {
        cout << "Book information: " << endl
             << *it << endl
             << "Do you want to delete? (enter y/yes to delete)\n> ";
        string confirm;
        cin.get();
        getline(cin, confirm);
        if (confirm == "y" || confirm == "yes") {
            if (it->GetBorrowStatus()) {
                cout << "The book is currently borrowed, Borrower: " << it->GetBorrower()
                     << ", Are you sure you want to delete? (enter y/yes to confirm)\n> ";
                getline(cin, confirm);
                if (confirm == "y" || confirm == "yes") {
                    libraryManager.erase(it);
                    cout << "Successfully deleted" << endl;
                } else {
                    cout << "Deletion cancelled" << endl;
                }
            } else {
                libraryManager.erase(it);
                cout << "Successfully deleted" << endl;
            }
        } else {
            cout << "Deletion cancelled" << endl;
        }
    } else {
        cout << "This book ID does not exist" << endl;
        cin.get();
    }

    cout << "Do you want to continue deleting? (enter y/yes to continue)\n> ";
    string confirm;
    getline(cin, confirm);
    if (confirm == "y" || confirm == "yes") {
        RemoveByID();
    }
}

void LibraryManager::RemoveByISBN() {
    string ISBN;
    cout << "Please enter the ISBN number of the book to delete: ";
    cin >> ISBN;

    bool find = false;
    auto it = libraryManager.begin();
    while (it != libraryManager.end()) {
        if (it->GetISBN() == ISBN) {
            find = true;
            break;
        }
        ++it;
    }
    if (find) {
        cout << "Book information: " << endl
             << "ISBN: " << it->GetISBN() << "  Book Name: " << it->GetName()
             << "  Author: " << it->GetAuthor()
             << "  Publisher: " << it->GetPublisher()
             << "  Publication Year: " << it->GetYear() << endl
             << "Do you want to delete? (enter y/yes to delete)\n> ";
        string confirm;
        cin.get();
        getline(cin, confirm);
        if (confirm == "y" || confirm == "yes") {
            while (it != libraryManager.end()) {
                auto itAfter = ++it;
                --it;
                if (it->GetISBN() == ISBN) {
                    if (it->GetBorrowStatus()) {
                        cout << "Book ID: " << it->GetId()
                             << " is borrowed, Borrower: " << it->GetBorrower()
                             << ", Do you want to delete? (enter y/yes to delete)\n> ";
                        getline(cin, confirm);
                        if (confirm == "y" || confirm == "yes") {
                            libraryManager.erase(it);
                        }
                    } else {
                        libraryManager.erase(it);
                    }
                }
                it = itAfter;
            }
            cout << "Deletion completed" << endl;
        } else {
            cout << "Deletion cancelled" << endl;
        }
    } else {
        cout << "This ISBN number does not exist" << endl;
        cin.get();
    }

    cout << "Do you want to continue deleting? (enter y/yes to continue)\n> ";
    string confirm;
    getline(cin, confirm);
    if (confirm == "y" || confirm == "yes") {
        RemoveByISBN();
    }
}

void LibraryManager::Lend() {
    int id;
    string borrower;

    cout << "Please enter the book ID to lend: ";
    cin >> id;
    auto it = libraryManager.find(id);
    if (it != libraryManager.end()) {
        if (it->GetBorrowStatus()) {
            cout << "This book is already lent out" << endl;
            cin.get();
        } else {
            cout << "Please enter the borrower's name: ";
            cin >> borrower;
            cout << "Please confirm the lending information (enter y/yes to confirm): " << endl
                 << "Book ID: " << id << "  ISBN: " << it->GetISBN()
                 << "  Book Name: " << it->GetName()
                 << "  Author: " << it->GetAuthor()
                 << "  Publisher: " << it->GetPublisher()
                 << "  Publication Year: " << it->GetYear() << "  Borrower: " << borrower
                 << "\n> ";
            string confirm;
            cin.get();
            getline(cin, confirm);
            if (confirm == "y" || confirm == "yes") {
                it->SetBorrowStatus(true);
                it->SetBorrower(borrower);
                cout << "Successfully lent out" << endl;
            } else {
                cout << "Lending cancelled" << endl;
            }
        }
    } else {
        cout << "This book ID does not exist" << endl;
        cin.get();
    }

    cout << "Do you want to continue lending? (enter y/yes to continue)\n> ";
    string confirm;
    getline(cin, confirm);
    if (confirm == "y" || confirm == "yes") {
        Lend();
    }
}

void LibraryManager::Return() {
    int id;
    cout << "Please enter the book ID to return: ";
    cin >> id;
    auto it = libraryManager.find(id);
    if (it != libraryManager.end()) {
        if (!it->GetBorrowStatus()) {
            cout << "This book is not lent out" << endl;
            cin.get();
        } else {
            cout << "Please confirm the return information (enter y/yes to confirm): " << endl
                 << "Book ID: " << id << "  ISBN: " << it->GetISBN()
                 << "  Book Name: " << it->GetName()
                 << "  Author: " << it->GetAuthor()
                 << "  Publisher: " << it->GetPublisher()
                 << "  Publication Year: " << it->GetYear()
                 << "  Borrower: " << it->GetBorrower() << "\n> ";
            string confirm;
            cin.get();
            getline(cin, confirm);
            if (confirm == "y" || confirm == "yes") {
                it->SetBorrowStatus(false);
                it->SetBorrower("");
                cout << "Successfully returned" << endl;
            } else {
                cout << "Return cancelled" << endl;
            }
        }
    } else {
        cout << "This book ID does not exist" << endl;
        cin.get();
    }

    cout << "Do you want to continue returning? (enter y/yes to continue)\n> ";
    string confirm;
    getline(cin, confirm);
    if (confirm == "y" || confirm == "yes") {
        Return();
    }
}

void LibraryManager::FindAllLend() {
    LibraryManager LendBook;
    auto it = libraryManager.begin();
    while (it != libraryManager.end()) {
        if (it->GetBorrowStatus()) {
            LendBook.libraryManager.insertUnique(LendBook.libraryManager.end(), *it);
        }
        ++it;
    }
    if (LendBook.Empty()) {
        cout << "Currently, there are no books lent out" << endl;
        return;
    }
    LendBook.FindByPage(1, 20);
}

void LibraryManager::Save(string filePath, string fileType) {
    ofstream out;
    out.open(filePath + ".temp", ios::app);
    if (!out.is_open()) {
        cout << "Failed to open file" << endl;
        return;
    }

    auto it = libraryManager.begin();
    while (it != libraryManager.end()) {
        out << *(it++) << endl;
    }

    out.close();
    std::remove((filePath + fileType).c_str());
    rename((filePath + ".temp").c_str(), (filePath + fileType).c_str());
}

