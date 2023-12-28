//
// Created by 王天赐 on 2023/12/24.
//

#ifndef LIBRARYBOOKMANAGESYSTEM_FILEOPERATION_H
#define LIBRARYBOOKMANAGESYSTEM_FILEOPERATION_H

#include <string>
#include "../include/libraryMgt.h"
#include "../include/adminMgt.h"
using namespace std;


class FileOperation {
private:
    string path;
    string fileType;

public:
    FileOperation(const string& _path, const string& _fileType);

    string getPath() {
        return path;
    }

    string getFileType() {
        return fileType;
    }
};


#endif //LIBRARYBOOKMANAGESYSTEM_FILEOPERATION_H
