ZJUT 2023 Fall Data Structure Lab by Tiancy

------------------------

# **图书馆书籍管理系统模拟**

## 一.  实验题目和要求

### 1.1 问题描述

在图书馆书籍管理系统中，管理员需要经常对图书信息进行查询和验证，如查询某本书是否在库、验证某本书的借阅状态等。由于图书信息的查询和验证频率很高，系统有必要有效地组织这些图书信息，从而快速查找和验证图书。另外，系统也会经常添加新入库的图书、删除损坏的图书和更新图书的借阅状态等操作，因此，系统必须采用动态结构，在添加、删除或更新后，依然能保证验证过程的快速。请采用相应的数据结构模拟图书馆书籍管理系统，其功能要求包括图书查询、图书状态更新、新书添加和旧书删除等。

 

### 1.2 基本要求

1. 要求自己编程实现二叉树结构及其相关功能，以存储图书信息，不允许使用标准模板类的二叉树结构和函数。同时要求根据二叉树的变化情况，进行相应的平衡操作，即AVL平衡树操作，四种平衡操作都必须考虑。测试时，各种情况都需要测试，并附上测试截图；
2. 要求采用类的设计思路，不允许出现类以外的函数定义，但允许友元函数；
3. 要求采用多文件方式：.h文件存储类的声明，.cpp文件存储类的实现，主函数main存储在另外一个单独的cpp文件中。如果采用类模板，则类的声明和实现都放在.h文件中；
4. 不强制要求采用类模板；不强制要求采用可视化窗口，但若采用可视化窗口可适当提高考核分数；要求源程序中有相应注释；
5. 要求测试例子要比较详尽，各种极限情况也要考虑到，测试的输出信息要详细易懂，表明各个功能的执行正确。



### 1.3 实现提示

1. 图书信息(即图书编号和借阅状态)可以存储在文件中，当程序启动时，从文件中读取所有的图书信息，并建立合适的查找二叉树；
2. 查询过程时，需要根据图书的编号，检索整个二叉树，找到匹配的图书编号，进行验证；更新图书借阅状态时，也需要检索二叉树，找到匹配项后进行更新，同时更新文件中存储的图书借阅状态；
3. 添加图书时，不仅需要在文件中添加，也需要在二叉树中添加相应的节点；删除图书时，也是如此；
4. 图书馆书籍管理系统可以联系实际，扩展相应功能。

 

### 1.4 运行结果要求

要求有程序菜单，能够实现图书管理员的登录管理、图书查询、添加图书、删除图书和更新图书借阅状态功能，实验报告要求有详细的功能测试截图。