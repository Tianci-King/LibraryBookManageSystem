#include "adminMgt.h"

#include <fstream>

AdminManager::AdminManager() = default;

AdminManager::~AdminManager() = default;

void AdminManager::Init(string path, string fileType) {
    string file = path + fileType;

    ifstream in;
    in.open(file, ios::in);
    if (!in.is_open()) {
        ofstream out;
        out.open(file, ios::out | ios::app);
        out.close();
        in.open(file, ios::in);
    }

    Admin user;
    while (in.peek() != EOF) {
        in >> user;
        adminManager.insertUnique(adminManager.end(), user);
    }

    in.close();
}

bool AdminManager::Register() {
    string secretKey, name, password, rePassword;

    cout << "Please enter the secret key: ";
    cin >> secretKey;
    if (secretKey != "vivo50") {
        cout << "Secret key is incorrect" << endl;
        return false;
    }

    cout << "Please enter a username: ";
    cin >> name;
    if (adminManager.find(name) != adminManager.end()) {
        cout << "This username is already registered" << endl;
        return false;
    }

    cout << "Please enter a password: ";
    cin >> password;
    cout << "Please confirm the password: ";
    cin >> rePassword;
    if (password != rePassword) {
        cout << "Passwords do not match" << endl;
        return false;
    }

    Admin user(name, password);
    adminManager.insertUnique(adminManager.end(), user);
    cout << "Registration successful" << endl;
    return true;
}

bool AdminManager::Login() {
    string name, password;
    cout << "Please enter your username: ";
    cin >> name;
    cout << "Please enter your password: ";
    cin >> password;

    auto it = adminManager.find(name);
    if (it == adminManager.end()) {
        cout << "Username or password is incorrect" << endl;
        return false;
    }

    if ((*it).GetPassword() != password) {
        cout << "Username or password is incorrect" << endl;
        return false;
    }

    cout << "Login successful" << endl;
    return true;
}

void AdminManager::Save(string filePath, string fileType) {
    ofstream out;
    out.open(filePath + ".temp", ios::app);
    if (!out.is_open()) {
        cout << "Failed to open file" << endl;
        return;
    }

    auto it = adminManager.begin();
    while (it != adminManager.end()) {
        out << *(it++) << endl;
    }

    out.close();
    remove((filePath + fileType).c_str());
    rename((filePath + ".temp").c_str(), (filePath + fileType).c_str());
}
