#ifndef _INCLUDE_TREE_H_
#define _INCLUDE_TREE_H_

#include <utility>  // pair

typedef bool Color;
const Color Red = false;
const Color Black = true;

// RB树节点
template <class Value>
struct Node {
    typedef Node<Value> *NodePtr;

    Color color;     // 节点颜色，非黑即红
    NodePtr parent;  // 父节点
    NodePtr left;    // 左子节点
    NodePtr right;   // 右子节点
    Value value;     // 节点值

    // 构造函数
    explicit Node(const Value &v = Value())
        : color(Red),
          parent(nullptr),
          left(nullptr),
          right(nullptr),
          value(v) {}

    // 求取极小值
    static NodePtr minimum(NodePtr x) {
        // 一直向左走就能找到极小值
        while (x->left != nullptr) {
            x = x->left;
        }
        return x;
    }

    // 求取极大值
    static NodePtr maximum(NodePtr x) {
        // 一直向右走就能极大值
        while (x->right != nullptr) {
            x = x->right;
        }
        return x;
    }
};

// 迭代器
template <class Value, class Ref, class Ptr>
struct Iterator {
    typedef Iterator<Value, Value &, Value *> iterator;
    typedef Iterator<Value, const Value &, const Value *> constIterator;
    typedef Iterator<Value, Ref, Ptr> Self;
    typedef Node<Value> *NodePtr;

    NodePtr node;

    // 默认构造函数
    Iterator() {}
    // 构造函数
    Iterator(NodePtr x) { node = x; }
    // 复制构造函数
    Iterator(const iterator &it) { node = it.node; }

    // 运算符重载
    Ref operator*() const { return node->value; }

    // 运算符重载
    Ptr operator->() const { return &(operator*()); }

    // 寻找后驱节点: 大于当前节点的最小节点
    void increment() {
        if (node->right != nullptr) {  // 如果有右子节点
            node = node->right;        // 就向右走
            // 然后往左子节点走到底时即为后驱节点
            while (node->left != nullptr) {
                node = node->left;
            }
        } else {                       // 没有右子节点
            NodePtr y = node->parent;  // 找出父节点
            // 如果当前节点是右子节点, 则一直上溯, 直到不为右子节点
            while (node == y->right) {
                node = y;
                y = y->parent;
            }
            if (node->right != y) {  // 如果此时的右子节点不等于父节点
                node = y;            // 则此时的父节点为后驱节点
            }                        // 否则当前节点为后驱节点
            // 以上判断"如果此时的右子节点不等于父节点", 是为了应对一种特殊情况
            // 即当我们想要寻找根节点的后驱节点时, 此时的根节点恰巧无右子节点
            // 以上特殊做法必须配合RB树根节点和其特殊的 header 之间的特殊关系
        }
    }

    // 寻找前驱节点: 小于当前节点的最大节点
    void decrement() {
        if (node->color == Red &&            // 如果是红节点, 且
            node->parent->parent == node) {  // 父节点的父节点等于当前节点
            node = node->right;  // 则此时的右子节点为前驱节点
            // 以上情况发生于 node 为 header 时 (也即 node 为 end() 时)
            // 注意, header 的右子节点即 mostright(), 指向整棵树的 max 节点
        } else if (node->left != nullptr) {  // 如果有左子节点
            NodePtr y = node->left;          // 令 y 指向左子节点
            // 然后往右子节点走到底时
            while (y->right != nullptr) {
                y = y->right;
            }
            node = y;                  // y 即为前驱节点
        } else {                       // 既非根节点, 也无左子节点
            NodePtr y = node->parent;  // 找出父节点
            // 如果当前节点是左子节点, 则一直上溯, 直到不为左子节点时
            while (node == y->left) {
                node = y;
                y = y->parent;
            }
            node = y;  // 当前节点的父节点即为前驱节点
        }
    }

    // 后置 ++
    Self &operator++() {
        increment();
        return *this;
    }
    // 前置 ++
    Self operator++(int) {
        Self tmp = *this;
        increment();
        return tmp;
    }

    // 后置 --
    Self &operator--() {
        decrement();
        return *this;
    }
    // 前置 --
    Self operator--(int) {
        Self tmp = *this;
        decrement();
        return tmp;
    }
};

// 运算符重载
template <class Value, class Ref, class Ptr>
bool operator==(const Iterator<Value, Ref, Ptr> &x,
                const Iterator<Value, Ref, Ptr> &y) {
    return x.node == y.node;
}
// 运算符重载
template <class Value, class Ref, class Ptr>
bool operator!=(const Iterator<Value, Ref, Ptr> &x,
                const Iterator<Value, Ref, Ptr> &y) {
    return x.node != y.node;
}

// 红黑树
template <class Key, class Value, class KeyOfValue, class Compare>
class RbTree {
public:
    typedef Node<Value> *NodePtr;
    typedef Node<Value> Node;
    typedef Value *Ptr;
    typedef const Value *constPtr;
    typedef Value &Ref;
    typedef const Value &constRef;

private:
    // RB树使用下面三个变量表示
    std::size_t nodeCount;  // 追踪记录树的大小 (节点数量)
    Compare keyCompare;  // 节点间的键值大小比较准则 (函数对象)
    NodePtr header;  // 实现上的一个技巧, 为了简化边界情况的处理
    // 注: header 和根节点互为父节点
    // header 的左子节点指向最左节点 (最小值)
    // 右子节点指向最右节点 (最大值)

    // 获取根节点 (即 header 的父节点)
    NodePtr &root() const { return header->parent; }
    // 获取最左节点 (即 header 的左子节点)
    NodePtr &leftmost() const { return header->left; }
    // 获取最右节点 (即 header 的右子节点)
    NodePtr &rightmost() const { return header->right; }

    // 用于节点的快速访问与设定
    // 获取节点 x 的左子节点
    static NodePtr &left(NodePtr x) { return x->left; }
    // 获取节点 x 的右子节点
    static NodePtr &right(NodePtr x) { return x->right; }
    // 获取节点 x 的父节点
    static NodePtr &parent(NodePtr x) { return x->parent; }
    // 获取节点 x 的值
    static Ref value(NodePtr x) { return x->value; }
    // 获取节点 x 的键
    static const Key &key(NodePtr x) { return KeyOfValue()(value(x)); }
    // 获取节点 x 的对象
    static Color &color(NodePtr x) { return (Color &)(x->color); }

    // 求取极小值
    static NodePtr minimum(NodePtr x) { return Node::minimum(x); }
    // 求取极大值
    static NodePtr maximum(NodePtr x) { return Node::maximum(x); }

    // 左旋
    // x 是旋转点, root 是根节点
    static void RotateLeft(NodePtr x, NodePtr &root);
    // 右旋
    // x 是旋转点, root 是根节点
    static void RotateRight(NodePtr x, NodePtr &root);
    // 重新令树平衡 (改变颜色及旋转)
    // x 是新节点, root 是根节点
    static void Rebalance(NodePtr x, NodePtr &root);
    // 移除节点后重新令树平衡
    // z 是要移除的节点, root 是根节点, leftmost 是最左节点, rightmost
    // 是最右节点 返回移除的节点 (已经脱离树)
    static NodePtr RebalanceForErase(NodePtr z, NodePtr &root,
                                     NodePtr &leftmost, NodePtr &rightmost);

public:
    typedef Iterator<Value, Ref, Ptr> iterator;
    typedef Iterator<Value, constRef, constPtr> constIterator;

private:
    // 真正的插入实现
    iterator _insert(NodePtr x, NodePtr y, const Value &v);
    // 移除以 x 为根节点的整棵子树, 并且不进行平衡操作
    void _erase(NodePtr x);
    // 复制一个节点的值和颜色
    NodePtr _cloneNode(NodePtr x);
    // 复制以 x 为根节点的子树至另一棵树的节点 p 下
    // 返回复制子树的根节点 (父节点为 p)
    NodePtr _copy(NodePtr x, NodePtr p);

public:
    // 默认构造函数
    RbTree(const Compare &comp = Compare()) : nodeCount(0), keyCompare(comp) {
        _emptyInitialize();
    }
    // 复制构造函数
    RbTree(const RbTree<Key, Value, KeyOfValue, Compare> &t)
        : nodeCount(0), keyCompare(t.keyCompare) {
        if (t.root() == 0) {     // 如果 t 是空树
            _emptyInitialize();  // 则初始化一棵空树
        } else {                 // 如果 t 非空树则进行复制
            color(header) = Red;
            root() = _copy(t.root(), header);  // 复制
            leftmost() = minimum(root());      // 设定最左节点
            rightmost() = maximum(root());     // 设定最右节点
        }
        nodeCount = t.nodeCount;  // 复制树的大小
    }

private:
    // 空树的初始化
    void _emptyInitialize() {
        header = new Node();  // 构造 header 节点
        color(header) =  // 令 header 为红色, 在 iterator.operator-- 中
            Red;         // 用来区分根节点 (根节点为黑色)
        root() = 0;      // 此时没有根节点
        leftmost() = header;   // 令 header 的左子节点为自己
        rightmost() = header;  // 令 header 的右子节点为自己
    }

public:
    // 析构函数
    ~RbTree() {
        clear();
        delete header;
    }

    // 运算符重载
    RbTree<Key, Value, KeyOfValue, Compare> &operator=(
        const RbTree<Key, Value, KeyOfValue, Compare> &x);

    // 返回指向起始的迭代器 (最左节点)
    iterator begin() { return leftmost(); }
    constIterator begin() const { return leftmost(); }
    // 返回指向末尾的迭代器
    // 因为前闭后开, 所以返回最右节点的后一个节点 (即 header)
    iterator end() { return header; }
    constIterator end() const { return header; }

    // 判断树是否为空
    bool empty() const { return nodeCount == 0; }
    // 返回树的大小 (即节点数量)
    std::size_t size() const { return nodeCount; }

    // 移除所有节点, header 回到初始状态
    void clear();

    // 插入新值, 节点键值不允许重复, 若重复则插入无效
    // 注: 返回值是 pair, 第一个元素是指向新增节点 (或重复键值节点) 的迭代器
    // 第二个元素表示插入是否成功
    std::pair<iterator, bool> insertUnique(const Value &v);
    // 插入新值, 节点键值允许重复
    // 返回指向新增节点的迭代器
    iterator insertEqual(const Value &v);

    // 在指定位置插入新值, 节点键值不允许重复, 若重复则插入无效
    // 逻辑是先判断插入位置是否正确, 正确则直接插入
    // 可以省去查找位置的时间, 错误则会先寻找到正确的位置再插入
    // 返回指向新增节点 (或重复键值节点) 的迭代器
    iterator insertUnique(iterator position, const Value &v);
    // 在指定位置插入新值, 节点键值允许重复
    // 逻辑同上
    // 返回指向新增节点的迭代器
    iterator insertEqual(iterator position, const Value &v);

    // 范围插入 range[first, last) 中的元素
    // 节点键值不允许重复, 若重复则插入无效
    template <class InputIterator>
    void insertUnique(InputIterator first, InputIterator last);
    // 范围插入 range[first, last) 中的元素, 节点键值允许重复
    template <class InputIterator>
    void insertEqual(InputIterator first, InputIterator last);

    // 移除指定位置的节点
    void erase(iterator position);
    // 移除键值为 k 的节点
    // 返回移除节点的数量
    std::size_t erase(const Key &k);
    // 移除 [first, last) 范围内的节点
    void erase(iterator first, iterator last);
    // 移除键值为 [first, last] 范围内的节点
    void erase(const Key *first, const Key *last);

    // 交换两棵树的内容
    // 即交换 header, nodeCount, keyCompare 三个用来表示一棵树的成员变量
    void swap(RbTree<Key, Value, KeyOfValue, Compare> &t);

    // 返回键值为 k 的结点的个数
    std::size_t count(const Key &k) const;

    // 寻找键值为 k 的节点的迭代器, 不存在则返回 end()
    iterator find(const Key &k);
    constIterator find(const Key &k) const;

    // 返回键值为 k 的节点区间
    // 注: 返回 pair, 第一个元素是首个 >= k 的节点的迭代器
    // 第二个元素是首个 > k 的节点的迭代器
    std::pair<iterator, iterator> equalRange(const Key &k);
    std::pair<constIterator, constIterator> equalRange(const Key &k) const;

    // 返回首个 >= k 的节点的迭代器
    iterator lowerBound(const Key &k);
    constIterator lowerBound(const Key &k) const;

    // 返回首个 > k 的节点的迭代器
    iterator upperBound(const Key &k);
    constIterator upperBound(const Key &k) const;

    // 返回节点间的键值大小比较准则 (函数对象)
    Compare keyComp() const { return keyCompare; }

    // 判断红黑树是否正确
    bool _rb_verify() const;

private:
    // 返回 node 节点到根节点路径上的黑色节点个数
    int _blackCount(NodePtr node, NodePtr root);
};

template <class Key, class Value, class KeyOfValue, class Compare>
typename RbTree<Key, Value, KeyOfValue, Compare>::iterator
RbTree<Key, Value, KeyOfValue, Compare>::_insert(NodePtr x, NodePtr y,
                                                 const Value &v) {
    // x 是新值的插入点, y 是插入点的父节点, v 是新值
    NodePtr z;

    // y == header: 此时是空树, 插入的节点作为根节点
    // x != 0
    // v < y: 新值小于父节点
    // 作为左子节点插入
    if (y == header || x != 0 || keyCompare(KeyOfValue()(v), key(y))) {
        z = new Node(v);
        left(y) = z;  // 设定父节点的左子节点
        // 同时当 y == header 时, leftmost() = z
        if (y == header) {  // 如果 y == header, 说明此时只有一个根节点
            root() = z;
            rightmost() = z;           // 因此最左最右节点都为 z
        } else if (y == leftmost()) {  // 如果 y 为最左节点
            leftmost() = z;  // z 作为 y 的左子节点, 成为新的 leftmost()
        }
    } else {  // 作为右子节点插入
        z = new Node(v);
        right(y) = z;            // 设定父节点的右子节点
        if (y == rightmost()) {  // 如果 y 为最右节点
            rightmost() = z;  // z 作为 y 的右子节点, 成为新的 rightmost()
        }
    }
    parent(z) = y;  // 设定新节点的父节点
    left(z) = 0;    // 设定新节点的左子节点
    right(z) = 0;   // 设定新节点的右子节点

    Rebalance(               // 重新令树平衡
        z, header->parent);  // 新节点的颜色在平衡时设定

    ++nodeCount;         // 节点数累加
    return iterator(z);  // 返回一个迭代器指向新增节点
}

// 移除以 x 为根节点的整棵子树, 并且不进行平衡操作
template <class Key, class Value, class KeyOfValue, class Compare>
void RbTree<Key, Value, KeyOfValue, Compare>::_erase(NodePtr x) {
    // 循环直至移除整棵子树
    while (x != 0) {
        _erase(right(x));  // 递归
        NodePtr y = left(x);
        delete x;
        x = y;
    }
}

template <class Key, class Value, class KeyOfValue, class Compare>
typename RbTree<Key, Value, KeyOfValue, Compare>::NodePtr
RbTree<Key, Value, KeyOfValue, Compare>::_cloneNode(NodePtr x) {
    NodePtr tmp = new Node(x->value);
    tmp->color = x->color;
    tmp->left = 0;
    tmp->right = 0;
    return tmp;
}

template <class Key, class Value, class KeyOfValue, class Compare>
typename RbTree<Key, Value, KeyOfValue, Compare>::NodePtr
RbTree<Key, Value, KeyOfValue, Compare>::_copy(NodePtr x, NodePtr p) {
    // top 作为当前复制树的根节点
    NodePtr top = _cloneNode(x);
    top->parent = p;
    // 复制右子树
    if (x->right != 0) {
        top->right = _copy(right(x), top);  // 递归
    }
    // 上面结束说明 top 的右子树已经复制完
    // 复制左子树
    p = top;
    x = left(x);
    while (x != 0) {
        // 复制左子节点
        NodePtr y = _cloneNode(x);
        p->left = y;
        y->parent = p;
        // 复制当前左子节点下的右子树
        if (x->right) {
            y->right = _copy(right(x), y);
        }
        p = y;
        x = left(x);
    }
    return top;
}

template <class Key, class Value, class KeyOfValue, class Compare>
RbTree<Key, Value, KeyOfValue, Compare>
    &RbTree<Key, Value, KeyOfValue, Compare>::operator=(
        const RbTree<Key, Value, KeyOfValue, Compare> &x) {
    if (this != &x) {
        clear();  // 移除所有节点, header 回到初始状态
        keyCompare = x.keyCompare;
        if (x.root() != 0) {  // 如果 x 非空树则进行复制
            root() = _copy(x.root(), header);  // 复制
            leftmost() = minimum(root());      // 设定最左节点
            rightmost() = maximum(root());     // 设定最右节点
            nodeCount = x.nodeCount;           // 复制树的大小
        }
    }
    return *this;
}

template <class Key, class Value, class KeyOfValue, class Compare>
void RbTree<Key, Value, KeyOfValue, Compare>::clear() {
    if (nodeCount != 0) {      // 如果不是空树
        _erase(root());        // 移除整棵树
        root() = 0;            // 此时没有根节点
        leftmost() = header;   // 回到 header 的初始状态
        rightmost() = header;  // 回到 header 的初始状态
        nodeCount = 0;         // 节点数量清零
    }
}

template <class Key, class Value, class KeyOfValue, class Compare>
std::pair<typename RbTree<Key, Value, KeyOfValue, Compare>::iterator, bool>
RbTree<Key, Value, KeyOfValue, Compare>::insertUnique(const Value &v) {
    NodePtr y = header;  // y 指向 x 的父节点
    NodePtr x = root();  // 从根节点开始
    bool comp = true;
    while (x != 0) {  // 往下寻找合适的插入点
        y = x;
        comp = keyCompare(KeyOfValue()(v), key(x));
        // v < x 则往左走, v >= x 则往右走
        x = comp ? left(x) : right(x);
    }
    iterator j = iterator(y);  // 令迭代器 j 指向插入节点的父节点
    if (comp) {                // 如果 comp 为 true, 表示将插入左侧
        if (j == begin()) {  // 如果插入节点的父节点是最左节点 (一直 <)
            return std::pair<iterator, bool>(
                _insert(x, y, v),  // 说明键值不重复
                true);
        } else {  // 如果不为最左节点 (出现过 >=), 则 --j <= v < j
            --j;  // 祖父节点可能 = v, --j 将 j 指向祖父节点后在下面进一步判断
        }
    }
    // 对于两种情况的判断
    // 一: comp = false, 表示将插入右侧, 此时 j 仍为父节点, 可能出现 j >= v
    // 二: comp = true, 此时 j 为祖父节点, 可能出现 j >= v
    if (keyCompare(key(j.node),
                   KeyOfValue()(v))) {  // 如果 j < v
        return std::pair<iterator, bool>(_insert(x, y, v),
                                         true);  // 说明键值不重复
    }
    // 到此处说明两种情况之一的 j = v 出现了
    // 即新值一定与树中的键值重复, 不插入新值
    return std::pair<iterator, bool>(j, false);
}

template <class Key, class Value, class KeyOfValue, class Compare>
typename RbTree<Key, Value, KeyOfValue, Compare>::iterator
RbTree<Key, Value, KeyOfValue, Compare>::insertEqual(const Value &v) {
    NodePtr y = header;  // y 指向 x 的父节点
    NodePtr x = root();  // 从根节点开始
    while (x != 0) {     // 往下寻找合适的插入点
        y = x;
        // v < x 则往左走, v >= x 则往右走
        x = keyCompare(KeyOfValue()(v), key(x)) ? left(x) : right(x);
    }
    // x 是新值插入点, y 是插入点的父节点, v 是新值
    // 注: header 和根节点互为父节点
    return _insert(x, y, v);
}

template <class Key, class Value, class KeyOfValue, class Compare>
typename RbTree<Key, Value, KeyOfValue, Compare>::iterator
RbTree<Key, Value, KeyOfValue, Compare>::insertUnique(iterator position,
                                                      const Value &v) {
    if (position.node == header->left) {       // 插入位置在 begin()
        if (size() > 0 &&                      // 非空树
            keyCompare(KeyOfValue()(v),        // 且 v < begin()
                       key(position.node))) {  // 非重复且位置正确
            return _insert(position.node, position.node, v);
        } else {  // 重复或位置不正确
            return insertUnique(v).first;
        }
    } else if (position.node == header) {   // 插入位置在 end()
        if (keyCompare(key(rightmost()),    // end() < v
                       KeyOfValue()(v))) {  // 非重复且位置正确
            return _insert(0, rightmost(), v);
        } else {  // 重复或位置不正确
            return insertUnique(v).first;
        }
    } else {  // 其他位置
        iterator before = position;
        --before;
        if (keyCompare(key(before.node), KeyOfValue()(v)) &&
            keyCompare(KeyOfValue()(v),        // before < v < position
                       key(position.node))) {  // 非重复且位置正确
            if (right(before.node) == 0) {
                return _insert(0, before.node, v);
            } else {
                return _insert(position.node, position.node, v);
            }
        } else {  // 重复或位置不正确
            return insertUnique(v).first;
        }
    }
}

template <class Key, class Value, class KeyOfValue, class Compare>
typename RbTree<Key, Value, KeyOfValue, Compare>::iterator
RbTree<Key, Value, KeyOfValue, Compare>::insertEqual(iterator position,
                                                     const Value &v) {
    if (position.node == header->left) {        // 插入位置在 begin()
        if (size() > 0                          // 非空树
            && !keyCompare(key(position.node),  // 且 begin() >= v
                           KeyOfValue()(v))) {  // 位置正确
            return _insert(position.node, position.node, v);
        } else {  // 位置不正确
            return insertEqual(v);
        }
    } else if (position.node == header) {     // 插入位置在 end()
        if (!keyCompare(KeyOfValue()(v),      // v >= end()
                        key(rightmost()))) {  // 位置正确
            return _insert(0, rightmost(), v);
        } else {
            return insertEqual(v);
        }
    } else {  // 其他位置
        iterator before = position;
        --before;
        if (!keyCompare(KeyOfValue()(v), key(before.node)) &&
            !keyCompare(key(position.node),  // position >= v >= before
                        KeyOfValue()(v))) {  // 位置正确
            if (right(before.node) == 0) {
                return _insert(0, before.node, v);
            } else {
                return _insert(position.node, position.node, v);
            }
        } else {  // 位置不正确
            return insertEqual(v);
        }
    }
}

template <class Key, class Value, class KeyOfValue, class Cmpare>
template <class InputIterator>
void RbTree<Key, Value, KeyOfValue, Cmpare>::insertUnique(InputIterator first,
                                                          InputIterator last) {
    for (; first != last; ++first) {
        insertUnique(*first);
    }
}

template <class Key, class Value, class KeyOfValue, class Cmpare>
template <class InputIterator>
void RbTree<Key, Value, KeyOfValue, Cmpare>::insertEqual(InputIterator first,
                                                         InputIterator last) {
    for (; first != last; ++first) {
        insertEqual(*first);
    }
}

template <class Key, class Value, class KeyOfValue, class Compare>
void RbTree<Key, Value, KeyOfValue, Compare>::erase(iterator position) {
    NodePtr y = RebalanceForErase(position.node, header->parent, header->left,
                                  header->right);
    delete y;
    --nodeCount;
}

template <class Key, class Value, class KeyOfValue, class Compare>
std::size_t RbTree<Key, Value, KeyOfValue, Compare>::erase(const Key &k) {
    std::pair<iterator, iterator> p = equalRange(k);
    std::size_t n = 0;
    for (iterator first = p.first; first != p.second; ++first, ++n)
        ;
    erase(p.first, p.second);
    return n;
}

template <class Key, class Value, class KeyOfValue, class Compare>
void RbTree<Key, Value, KeyOfValue, Compare>::erase(iterator first,
                                                    iterator last) {
    if (first == begin() && last == end()) {  // 等价于移除所有节点
        clear();
    } else {
        while (first != last) {
            erase(first++);
        }
    }
}

template <class Key, class Value, class KeyOfValue, class Compare>
void RbTree<Key, Value, KeyOfValue, Compare>::erase(const Key *first,
                                                    const Key *last) {
    while (first != last) {
        erase(*first++);
    }
}

template <class Key, class Value, class KeyOfValue, class Compare>
void RbTree<Key, Value, KeyOfValue, Compare>::swap(
    RbTree<Key, Value, KeyOfValue, Compare> &t) {
    std::swap(header, t.header);
    std::swap(nodeCount, t.nodeCount);
    std::swap(keyCompare, t.keyCompare);
}

template <class Key, class Value, class KeyOfValue, class Compare>
std::size_t RbTree<Key, Value, KeyOfValue, Compare>::count(const Key &k) const {
    std::pair<constIterator, constIterator> p = equalRange(k);
    std::size_t n = 0;
    for (iterator first = p.first; first != p.second; ++first, ++n)
        ;
    return n;
}

template <class Key, class Value, class KeyOfValue, class Compare>
typename RbTree<Key, Value, KeyOfValue, Compare>::iterator
RbTree<Key, Value, KeyOfValue, Compare>::find(const Key &k) {
    NodePtr y = header;  // 最终指向首个 >= k 的节点
    NodePtr x = root();  // 当前节点

    while (x != 0) {                   // 非空树
        if (!keyCompare(key(x), k)) {  // k <= x
            y = x, x = left(x);
        } else {  // k > x
            x = right(x);
        }
    }

    iterator j = iterator(y);
    // 如果不存在键值为 k 的节点则返回 end()
    return (j == end() || keyCompare(k, key(j.node))) ? end() : j;
}

template <class Key, class Value, class KeyOfValue, class Compare>
typename RbTree<Key, Value, KeyOfValue, Compare>::constIterator
RbTree<Key, Value, KeyOfValue, Compare>::find(const Key &k) const {
    NodePtr y = header;  // 最终指向首个 >= k 的节点
    NodePtr x = root();  // 当前节点

    while (x != 0) {                   // 非空树
        if (!keyCompare(key(x), k)) {  // k <= x
            y = x, x = left(x);
        } else {  // k > x
            x = right(x);
        }
    }

    constIterator j = constIterator(y);
    // 如果不存在键值为 k 的节点则返回 end()
    return (j == end() || keyCompare(k, key(j.node))) ? end() : j;
}

template <class Key, class Value, class KeyOfValue, class Compare>
typename RbTree<Key, Value, KeyOfValue, Compare>::iterator
RbTree<Key, Value, KeyOfValue, Compare>::lowerBound(const Key &k) {
    NodePtr y = header;  // 最终指向首个 >= k 的节点
    NodePtr x = root();  // 当前节点

    while (x != 0) {                   // 非空树
        if (!keyCompare(key(x), k)) {  // k <= x
            y = x, x = left(x);
        } else {  // k > x
            x = right(x);
        }
    }

    return iterator(y);
}

template <class Key, class Value, class KeyOfValue, class Compare>
typename RbTree<Key, Value, KeyOfValue, Compare>::constIterator
RbTree<Key, Value, KeyOfValue, Compare>::lowerBound(const Key &k) const {
    NodePtr y = header;  // 最终指向首个 >= k 的节点
    NodePtr x = root();  // 当前节点

    while (x != 0) {                   // 非空树
        if (!keyCompare(key(x), k)) {  // k <= x
            y = x, x = left(x);
        } else {  // k > x
            x = right(x);
        }
    }

    return constIterator(y);
}

template <class Key, class Value, class KeyOfValue, class Compare>
typename RbTree<Key, Value, KeyOfValue, Compare>::iterator
RbTree<Key, Value, KeyOfValue, Compare>::upperBound(const Key &k) {
    NodePtr y = header;  // 最终指向首个 > k 的节点
    NodePtr x = root();  // 当前节点

    while (x != 0) {                  // 非空树
        if (keyCompare(k, key(x))) {  // k < x
            y = x, x = left(x);
        } else {  // k >= x
            x = right(x);
        }
    }

    return iterator(y);
}

template <class Key, class Value, class KeyOfValue, class Compare>
typename RbTree<Key, Value, KeyOfValue, Compare>::constIterator
RbTree<Key, Value, KeyOfValue, Compare>::upperBound(const Key &k) const {
    NodePtr y = header;  // 最终指向首个 > k 的节点
    NodePtr x = root();  // 当前节点

    while (x != 0) {                  // 非空树
        if (keyCompare(k, key(x))) {  // k < x
            y = x, x = left(x);
        } else {  // k >= x
            x = right(x);
        }
    }

    return constIterator(y);
}

template <class Key, class Value, class KeyOfValue, class Compare>
std::pair<typename RbTree<Key, Value, KeyOfValue, Compare>::iterator,
          typename RbTree<Key, Value, KeyOfValue, Compare>::iterator>
RbTree<Key, Value, KeyOfValue, Compare>::equalRange(const Key &k) {
    return std::pair<iterator, iterator>(lowerBound(k), upperBound(k));
}

template <class Key, class Value, class KeyOfValue, class Compare>
std::pair<typename RbTree<Key, Value, KeyOfValue, Compare>::constIterator,
          typename RbTree<Key, Value, KeyOfValue, Compare>::constIterator>
RbTree<Key, Value, KeyOfValue, Compare>::equalRange(const Key &k) const {
    return std::pair<constIterator, constIterator>(lowerBound(k),
                                                   upperBound(k));
}

template <class Key, class Value, class KeyOfValue, class Compare>
bool RbTree<Key, Value, KeyOfValue, Compare>::_rb_verify() const {
    // 判断是否是合法的空树
    if (nodeCount == 0 || begin() == end()) {
        return nodeCount == 0 && begin() == end() && header->left == header &&
               header->right == header;
    }

    int len = _blackCount(leftmost(), root());
    // 循环遍历节点判断是否合法
    for (constIterator it = begin(); it != end(); ++it) {
        NodePtr x = it.node;
        NodePtr L = left(x);
        NodePtr R = right(x);

        // 判断是否有两个连续红色节点
        if (x->color == Red) {
            if ((L && L->color == Red) || (R && R->color == Red)) {
                return false;
            }
        }

        // 判断节点位置是否正确
        if (L && keyCompare(key(x), key(L))) {
            return false;
        }
        if (R && keyCompare(key(R), key(x))) {
            return false;
        }

        // 判断叶子节点到根节点的黑色节点个数是否相同
        if (!L && !R && _blackCount(x, root()) != len) {
            return false;
        }
    }

    // 判断最左最右节点是否正确
    if (leftmost() != Node::minimum(root())) {
        return false;
    }
    if (rightmost() != Node::maximum(root())) {
        return false;
    }

    return true;
}

template <class Key, class Value, class KeyOfValue, class Compare>
int RbTree<Key, Value, KeyOfValue, Compare>::_blackCount(NodePtr node,
                                                         NodePtr root) {
    if (node == nullptr) {
        return 0;
    } else {
        int bc = node->color == Black ? 1 : 0;
        if (node == root) {
            return bc;
        } else {
            return bc + _blackCount(node->parent, root);
        }
    }
}

template <class Key, class Value, class KeyOfValue, class Compare>
void RbTree<Key, Value, KeyOfValue, Compare>::RotateLeft(NodePtr x,
                                                         NodePtr &root) {
    NodePtr y = x->right;      // 令 y 为旋转点的右子节点
    x->right = y->left;        // x 的右子节点更改为 y 的左子节点
    if (y->left != nullptr) {  // 如果 y 的左子节点存在
        y->left->parent = x;   // 更改其左子节点的父节点为 x
    }
    y->parent = x->parent;  // y 的父节点更改为 x 的父节点

    // 令 y 完全替代 x (接收 x 对于其父节点的关系)
    if (x == root) {  // 如果 x 是根节点 (此时 y 的父节点是 header)
        root = y;
    } else if (x == x->parent->left) {  // 如果 x 是父节点的左子节点
        x->parent->left = y;            // x 父节点的左子节点更改为 y
    } else {                            // 如果 x 是父节点的右子节点
        x->parent->right = y;           // x 父节点的右子节点更改为 y
    }
    y->left = x;
    x->parent = y;
}

template <class Key, class Value, class KeyOfValue, class Compare>
void RbTree<Key, Value, KeyOfValue, Compare>::RotateRight(NodePtr x,
                                                          NodePtr &root) {
    NodePtr y = x->left;        // 令 y 为旋转点的左子节点
    x->left = y->right;         // x 的左子节点更改为 y 的右子节点
    if (y->right != nullptr) {  // 如果 y 的右子节点存在
        y->right->parent = x;   // 更改其右子节点的父节点为 x
    }
    y->parent = x->parent;  // y 的父节点更改为 x 的父节点

    // 令 y 完全替代 x (接收 x 对于其父节点的关系)
    if (x == root) {  // 如果 x 是根节点 (此时 y 的父节点是 header)
        root = y;     // 根节点更改为 y
    } else if (x == x->parent->right) {  // 如果 x 是父节点的右子节点
        x->parent->right = y;  // x 父节点的右子节点更改为 y
    } else {                   // 如果 x 是父节点的左子节点
        x->parent->left = y;   // x 父节点的左子节点更改为 y
    }
    y->right = x;   // y 的右子节点更改为 x
    x->parent = y;  // x 的父子结点更改为 y
}

template <class Key, class Value, class KeyOfValue, class Compare>
void RbTree<Key, Value, KeyOfValue, Compare>::Rebalance(NodePtr x,
                                                        NodePtr &root) {
    // 设置新节点颜色为红色, 因为如果插入的节点是黑色, 必然会导致树不平衡
    x->color = Red;
    // 当 x 非根节点且 x 的父节点为红色时需要进行平衡操作
    while (x != root && x->parent->color == Red) {
        // 一: 父节点是祖父节点的左子节点
        if (x->parent == x->parent->parent->left) {
            // 令 y 为伯父节点
            NodePtr y = x->parent->parent->right;
            // 1: 伯父节点存在且为红色
            if (y && y->color == Red) {
                // 更改父节点为黑色
                x->parent->color = Black;
                // 更改伯父节点为黑色
                y->color = Black;
                // 更改祖父节点为红色
                x->parent->parent->color = Red;
                // 祖父颜色由黑变红可能会破坏上层平衡
                // 将当前节点更改为祖父节点继续往上检查
                x = x->parent->parent;
            } else {  // 2: 伯父节点不存在或为黑色
                // 实际上是同种情况, 因为 nullptr 被看作虚拟黑色子节点
                // (1): 新节点是父节点的右子节点
                // 需要先进行左旋成为左子节点
                if (x == x->parent->right) {
                    x = x->parent;
                    // 左旋
                    RotateLeft(x, root);
                }
                // (2): 新节点是父节点的左子节点
                // 更改父节点为黑色
                x->parent->color = Black;
                // 更改祖父节点为红色f
                x->parent->parent->color = Red;
                // 右旋
                RotateRight(x->parent->parent, root);
            }
        } else {  // 二: 父节点是祖父节点的右子节点
            // 令 y 为伯父节点
            NodePtr y = x->parent->parent->left;
            // 1: 伯父节点存在且为红色
            if (y && y->color == Red) {
                // 更改父节点为黑色
                x->parent->color = Black;
                // 更改伯父节点为黑色
                y->color = Black;
                // 更改祖父节点为红色
                x->parent->parent->color = Red;
                // 祖父颜色由黑变红可能会破坏上层平衡
                // 将当前节点更改为祖父节点继续往上检查
                x = x->parent->parent;
            } else {  // 2: 伯父节点不存在或为黑色
                // 实际上是同种情况, 因为 nullptr 被看作虚拟黑色子节点
                // (1): 新节点是父节点的左子节点
                // 需要先进行右旋成为右子节点
                if (x == x->parent->left) {
                    x = x->parent;
                    // 右旋
                    RotateRight(x, root);
                }
                // (2): 新节点是父节点的右子节点
                // 更改父节点为黑色
                x->parent->color = Black;
                // 更改祖父节点为红色
                x->parent->parent->color = Red;
                // 左旋
                RotateLeft(x->parent->parent, root);
            }
        }
    }
    root->color = Black;  // 根节点永远为黑色
}

template <class Key, class Value, class KeyOfValue, class Compare>
typename RbTree<Key, Value, KeyOfValue, Compare>::NodePtr
RbTree<Key, Value, KeyOfValue, Compare>::RebalanceForErase(NodePtr z,
                                                           NodePtr &root,
                                                           NodePtr &leftmost,
                                                           NodePtr &rightmost) {
    NodePtr y = z;
    NodePtr x = nullptr;  // x 指向的是实际移除节点的子节点
    NodePtr xParent = nullptr;

    if (y->left == nullptr) {          // z 没有左子节点
        x = y->right;                  // x 指向右子节点 (可能为空)
    } else if (y->right == nullptr) {  // z 只有左子节点
        x = y->left;                   // x 指向左子节点
    } else {                           // z 有两个子节点
        // y 指向移除节点的后驱节点
        y = y->right;
        while (y->left != nullptr) {
            y = y->left;
        }
        x = y->right;  // 指向后驱节点的右子节点 (可能为空)
    }
    if (y != z) {  // z 有两个子节点
        // 用 y (后驱节点) 取代 z (移除节点) 的位置
        // 等同于移除 y
        // 同时调整 y 的右子节点 (即 x) 的位置 (没有左子节点)
        z->left->parent = y;  // 取代左子节点
        y->left = z->left;
        if (y != z->right) {  // 不是右子节点则取代右子节点
            xParent = y->parent;
            // 下面四行调整 x 的位置
            if (x) {
                x->parent = y->parent;
            }
            y->parent->left = x;
            y->right = z->right;  // 取代右子节点
            z->right->parent = y;
        } else {  // 本身就是右子节点不需要再取代
            xParent = y;
        }
        // 取代父节点
        if (root == z) {  // 如果移除的是根节点
            root = y;     // y 还需要继承根节点的特殊位置
        } else if (z->parent->left == z) {
            z->parent->left = y;
        } else {
            z->parent->right = y;
        }
        y->parent = z->parent;
        std::swap(y->color, z->color);  // 交换颜色
        y = z;                          // y 重新指向需要移除的节点
    } else {  // z 只有一个子结点 (即 x), 用 x 取代 z 的位置
        xParent = y->parent;
        if (x) {
            x->parent = y->parent;
        }
        if (root == z) {  // 如果移除的是根节点
            root = x;     // x 还需要继承根节点的特殊位置
        } else if (z->parent->left == z) {
            z->parent->left = x;
        } else {
            z->parent->right = x;
        }
        if (leftmost == z) {  // 如果移除的是最左节点, 维护 leftmost 的位置
            if (z->right == nullptr) {
                leftmost = z->parent;
            } else {
                leftmost = Node::minimum(x);
            }
        }
        if (rightmost == z) {  // 如果移除的是最左节点, 维护 rightmost 的位置
            if (z->left == nullptr) {
                rightmost = z->parent;
            } else {
                rightmost = Node::maximum(x);
            }
        }
    }
    // 重新平衡
    // 删除红色节点不会破坏平衡
    if (y->color != Red) {
        // x 非根节点并且 x 为空或红色, 即少一个黑色节点
        while (x != root && (x == nullptr || x->color == Black)) {
            if (x == xParent->left) {        // 移除的是左子节点
                NodePtr w = xParent->right;  // w 指向 x 的兄弟节点
                // 一: 兄弟节点为红色
                // 1.将兄弟节点变为黑色
                // 2.将父节点变为红色
                // 3.对父节点进行左旋
                // 4.左旋后重新设置 x 的兄弟节点
                // 此时 x 还是少一个黑色节点, 进入另一种状态继续调整
                if (w->color == Red) {
                    w->color = Black;
                    xParent->color = Red;
                    RotateLeft(xParent, root);
                    w = xParent->right;
                }
                // 二: 兄弟节点为黑色, 兄弟节点的两个子节点也都为黑色
                // 1.将兄弟节点变为红色
                // 2.设置 x 的父节点为新的 x 节点
                // 此时 x 的父节点整体少一个黑色节点, 成为新的 x 节点
                if ((w->left == nullptr || w->left->color == Black) &&
                    (w->right == nullptr || w->right->color == Black)) {
                    w->color = Red;
                    x = xParent;
                    xParent = xParent->parent;
                } else {
                    // 三: 兄弟节点为黑色, 其左子节点为红色, 右子节点为黑色
                    // 1.将兄弟节点的左子节点变为黑色
                    // 2.将兄弟节点变为红色
                    // 3.对兄弟节点进行右旋
                    // 4.右旋后重新设置 x 的兄弟节点
                    // 此时到达情况四
                    if (w->right == nullptr || w->right->color == Black) {
                        if (w->left) {
                            w->left->color = Black;
                        }
                        w->color = Red;
                        RotateRight(w, root);
                        w = xParent->right;
                    }
                    // 四: 兄弟节点为黑色, 其右子节点为红色
                    // 1.将兄弟节点变为父节点的颜色
                    // 2.将父节点变为黑色
                    // 3.将兄弟节点的右子节点变为黑色
                    // 4.对父节点进行左旋
                    // 完成平衡, 退出循环
                    w->color = xParent->color;
                    xParent->color = Black;
                    if (w->right) {
                        w->right->color = Black;
                    }
                    RotateLeft(xParent, root);
                    break;
                }
            } else {  // 移除的是右子节点, 与上面的平衡过程对应
                NodePtr w = xParent->left;
                if (w->color == Red) {
                    w->color = Black;
                    xParent->color = Red;
                    RotateRight(xParent, root);
                    w = xParent->left;
                }
                if ((w->right == nullptr || w->right->color == Black) &&
                    (w->left == nullptr || w->left->color == Black)) {
                    w->color = Red;
                    x = xParent;
                    xParent = xParent->parent;
                } else {
                    if (w->left == nullptr || w->left->color == Black) {
                        if (w->right) {
                            w->right->color = Black;
                        }
                        w->color = Red;
                        RotateLeft(w, root);
                        w = xParent->left;
                    }
                    w->color = xParent->color;
                    xParent->color = Black;
                    if (w->left) {
                        w->left->color = Black;
                    }
                    RotateRight(xParent, root);
                    break;
                }
            }
        }
        if (x) {
            x->color = Black;
        }
    }
    return y;
}

#endif  // _INCLUDE_TREE_H_