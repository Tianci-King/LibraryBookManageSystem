#include "menuPrinter.h"

#include <iostream>
using namespace std;

void Menu::Start() {
    cout << "----------------------------------------------------------------"
         << endl
         << "Welcome to the Library Book Management System" << endl
         << "1: Log in" << endl
         << "2: Register" << endl
         << "0: Exit" << endl
         << "> ";
}

void Menu::Main() {
    cout << "----------------------------------------------------------------"
         << endl
         << "Please enter a number to operate" << endl
         << "1: Add Book" << endl
         << "2: Search for Book" << endl
         << "3: Modify Book" << endl
         << "4: Delete Book" << endl
         << "5: Borrowing Management" << endl
         << "0: Logout" << endl
         << "> ";
}

void Menu::Find() {
    cout << "----------------------------------------------------------------"
         << endl
         << "Please enter a number to operate" << endl
         << "1: Search by Book Number" << endl
         << "2: Search by ISBN" << endl
         << "3: View All Books" << endl
         << "0: Return to Previous Menu" << endl
         << "> ";
}

void Menu::Update() {
    cout << "----------------------------------------------------------------"
         << endl
         << "Please enter the corresponding number to operate" << endl
         << "1: Modify by Book Number" << endl
         << "2: Modify by ISBN" << endl
         << "0: Return to Previous Menu" << endl
         << "> ";
}

void Menu::Remove() {
    cout << "----------------------------------------------------------------"
         << endl
         << "Please enter a number to operate" << endl
         << "1: Delete by Book Number" << endl
         << "2: Delete by ISBN" << endl
         << "0: Return to Previous Menu" << endl
         << "> ";
}

void Menu::LendOrReturn() {
    cout << "----------------------------------------------------------------"
         << endl
         << "Please enter a number to operate" << endl
         << "1: Lend Out" << endl
         << "2: Return" << endl
         << "3: View All Lent Out Books" << endl
         << "0: Return to Previous Menu" << endl
         << "> ";
}
