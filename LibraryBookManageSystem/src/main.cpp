#include "libraryMgt.h"
#include "menuPrinter.h"
#include "adminMgt.h"
#include "FileOperation.h"

int main() {
    AdminManager admin;
    LibraryManager book;
    FileOperation adminFile("../data/admin", ".txt");
    FileOperation bookFile("../data/book", ".txt");
    admin.Init(adminFile.getPath(), adminFile.getFileType());
    book.Init(bookFile.getPath(), bookFile.getFileType());

    Menu::Start();
    int choice;
    while (cin >> choice && choice) {
        switch (choice) {
            case 1:
                if (!admin.Login()) {
                    break;
                }

                Menu::Main();
                while (cin >> choice && choice) {
                    switch (choice) {
                        case 1: {
                            book.Insert();
                            break;
                        }
                        case 2: {
                            if (book.Empty()) {
                                cout << "The current book inventory is empty" << endl;
                                break;
                            }
                            Menu::Find();
                            while (cin >> choice && choice) {
                                switch (choice) {
                                    case 1: {
                                        book.FindByID();
                                        break;
                                    }
                                    case 2: {
                                        book.FindByISBN();
                                        break;
                                    }
                                    case 3: {
                                        book.FindByPage(1, 20);
                                        break;
                                    }
                                    default: {
                                        cout << "Invalid input, please re-enter" << endl;
                                        break;
                                    }
                                }
                                Menu::Find();
                            }
                            break;
                        }
                        case 3: {
                            if (book.Empty()) {
                                cout << "The current book inventory is empty" << endl;
                                break;
                            }
                            Menu::Update();
                            while (cin >> choice && choice) {
                                switch (choice) {
                                    case 1: {
                                        book.UpdateByID();
                                        break;
                                    }
                                    case 2: {
                                        book.UpdateByISBN();
                                        break;
                                    }
                                    default: {
                                        cout << "Invalid input, please re-enter" << endl;
                                        break;
                                    }
                                }
                                Menu::Update();
                            }
                            break;
                        }
                        case 4: {
                            if (book.Empty()) {
                                cout << "The current book inventory is empty" << endl;
                                break;
                            }
                            Menu::Remove();
                            while (cin >> choice && choice) {
                                switch (choice) {
                                    case 1: {
                                        book.RemoveByID();
                                        break;
                                    }
                                    case 2: {
                                        book.RemoveByISBN();
                                        break;
                                    }
                                    default: {
                                        cout << "Invalid input, please re-enter" << endl;
                                        break;
                                    }
                                }
                                Menu::Remove();
                            }
                            break;
                        }
                        case 5: {
                            Menu::LendOrReturn();
                            while (cin >> choice && choice) {
                                switch (choice) {
                                    case 1: {
                                        book.Lend();
                                        break;
                                    }
                                    case 2: {
                                        book.Return();
                                        break;
                                    }
                                    case 3: {
                                        book.FindAllLend();
                                        break;
                                    }
                                    default: {
                                        cout << "Invalid input, please re-enter" << endl;
                                        break;
                                    }
                                }
                                Menu::LendOrReturn();
                            }
                            break;
                        }
                        default: {
                            cout << "Invalid input, please re-enter" << endl;
                            break;
                        }
                    }
                    Menu::Main();
                }
                break;
            case 2: {
                admin.Register();
                break;
            }
            default: {
                cout << "Invalid input, please re-enter" << endl;
                break;
            }
        }
        Menu::Start();
    }

    admin.Save(adminFile.getPath(), adminFile.getFileType());
    book.Save(bookFile.getPath(), bookFile.getFileType());
    cout << "Welcome to use again next time~" << endl;

    return 0;
}