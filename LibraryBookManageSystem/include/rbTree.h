#ifndef _INCLUDE_TREE_H_
#define _INCLUDE_TREE_H_

#include <utility>  // pair

typedef bool Color;
const Color Red = false;
const Color Black = true;

// RB���ڵ�
template <class Value>
struct Node {
    typedef Node<Value> *NodePtr;

    Color color;     // �ڵ���ɫ���Ǻڼ���
    NodePtr parent;  // ���ڵ�
    NodePtr left;    // ���ӽڵ�
    NodePtr right;   // ���ӽڵ�
    Value value;     // �ڵ�ֵ

    // ���캯��
    explicit Node(const Value &v = Value())
        : color(Red),
          parent(nullptr),
          left(nullptr),
          right(nullptr),
          value(v) {}

    // ��ȡ��Сֵ
    static NodePtr minimum(NodePtr x) {
        // һֱ�����߾����ҵ���Сֵ
        while (x->left != nullptr) {
            x = x->left;
        }
        return x;
    }

    // ��ȡ����ֵ
    static NodePtr maximum(NodePtr x) {
        // һֱ�����߾��ܼ���ֵ
        while (x->right != nullptr) {
            x = x->right;
        }
        return x;
    }
};

// ������
template <class Value, class Ref, class Ptr>
struct Iterator {
    typedef Iterator<Value, Value &, Value *> iterator;
    typedef Iterator<Value, const Value &, const Value *> constIterator;
    typedef Iterator<Value, Ref, Ptr> Self;
    typedef Node<Value> *NodePtr;

    NodePtr node;

    // Ĭ�Ϲ��캯��
    Iterator() {}
    // ���캯��
    Iterator(NodePtr x) { node = x; }
    // ���ƹ��캯��
    Iterator(const iterator &it) { node = it.node; }

    // ���������
    Ref operator*() const { return node->value; }

    // ���������
    Ptr operator->() const { return &(operator*()); }

    // Ѱ�Һ����ڵ�: ���ڵ�ǰ�ڵ����С�ڵ�
    void increment() {
        if (node->right != nullptr) {  // ��������ӽڵ�
            node = node->right;        // ��������
            // Ȼ�������ӽڵ��ߵ���ʱ��Ϊ�����ڵ�
            while (node->left != nullptr) {
                node = node->left;
            }
        } else {                       // û�����ӽڵ�
            NodePtr y = node->parent;  // �ҳ����ڵ�
            // �����ǰ�ڵ������ӽڵ�, ��һֱ����, ֱ����Ϊ���ӽڵ�
            while (node == y->right) {
                node = y;
                y = y->parent;
            }
            if (node->right != y) {  // �����ʱ�����ӽڵ㲻���ڸ��ڵ�
                node = y;            // ���ʱ�ĸ��ڵ�Ϊ�����ڵ�
            }                        // ����ǰ�ڵ�Ϊ�����ڵ�
            // �����ж�"�����ʱ�����ӽڵ㲻���ڸ��ڵ�", ��Ϊ��Ӧ��һ���������
            // ����������ҪѰ�Ҹ��ڵ�ĺ����ڵ�ʱ, ��ʱ�ĸ��ڵ�ǡ�������ӽڵ�
            // �������������������RB�����ڵ��������� header ֮��������ϵ
        }
    }

    // Ѱ��ǰ���ڵ�: С�ڵ�ǰ�ڵ�����ڵ�
    void decrement() {
        if (node->color == Red &&            // ����Ǻ�ڵ�, ��
            node->parent->parent == node) {  // ���ڵ�ĸ��ڵ���ڵ�ǰ�ڵ�
            node = node->right;  // ���ʱ�����ӽڵ�Ϊǰ���ڵ�
            // ������������� node Ϊ header ʱ (Ҳ�� node Ϊ end() ʱ)
            // ע��, header �����ӽڵ㼴 mostright(), ָ���������� max �ڵ�
        } else if (node->left != nullptr) {  // ��������ӽڵ�
            NodePtr y = node->left;          // �� y ָ�����ӽڵ�
            // Ȼ�������ӽڵ��ߵ���ʱ
            while (y->right != nullptr) {
                y = y->right;
            }
            node = y;                  // y ��Ϊǰ���ڵ�
        } else {                       // �ȷǸ��ڵ�, Ҳ�����ӽڵ�
            NodePtr y = node->parent;  // �ҳ����ڵ�
            // �����ǰ�ڵ������ӽڵ�, ��һֱ����, ֱ����Ϊ���ӽڵ�ʱ
            while (node == y->left) {
                node = y;
                y = y->parent;
            }
            node = y;  // ��ǰ�ڵ�ĸ��ڵ㼴Ϊǰ���ڵ�
        }
    }

    // ���� ++
    Self &operator++() {
        increment();
        return *this;
    }
    // ǰ�� ++
    Self operator++(int) {
        Self tmp = *this;
        increment();
        return tmp;
    }

    // ���� --
    Self &operator--() {
        decrement();
        return *this;
    }
    // ǰ�� --
    Self operator--(int) {
        Self tmp = *this;
        decrement();
        return tmp;
    }
};

// ���������
template <class Value, class Ref, class Ptr>
bool operator==(const Iterator<Value, Ref, Ptr> &x,
                const Iterator<Value, Ref, Ptr> &y) {
    return x.node == y.node;
}
// ���������
template <class Value, class Ref, class Ptr>
bool operator!=(const Iterator<Value, Ref, Ptr> &x,
                const Iterator<Value, Ref, Ptr> &y) {
    return x.node != y.node;
}

// �����
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
    // RB��ʹ����������������ʾ
    std::size_t nodeCount;  // ׷�ټ�¼���Ĵ�С (�ڵ�����)
    Compare keyCompare;  // �ڵ��ļ�ֵ��С�Ƚ�׼�� (��������)
    NodePtr header;  // ʵ���ϵ�һ������, Ϊ�˼򻯱߽�����Ĵ���
    // ע: header �͸��ڵ㻥Ϊ���ڵ�
    // header �����ӽڵ�ָ������ڵ� (��Сֵ)
    // ���ӽڵ�ָ�����ҽڵ� (���ֵ)

    // ��ȡ���ڵ� (�� header �ĸ��ڵ�)
    NodePtr &root() const { return header->parent; }
    // ��ȡ����ڵ� (�� header �����ӽڵ�)
    NodePtr &leftmost() const { return header->left; }
    // ��ȡ���ҽڵ� (�� header �����ӽڵ�)
    NodePtr &rightmost() const { return header->right; }

    // ���ڽڵ�Ŀ��ٷ������趨
    // ��ȡ�ڵ� x �����ӽڵ�
    static NodePtr &left(NodePtr x) { return x->left; }
    // ��ȡ�ڵ� x �����ӽڵ�
    static NodePtr &right(NodePtr x) { return x->right; }
    // ��ȡ�ڵ� x �ĸ��ڵ�
    static NodePtr &parent(NodePtr x) { return x->parent; }
    // ��ȡ�ڵ� x ��ֵ
    static Ref value(NodePtr x) { return x->value; }
    // ��ȡ�ڵ� x �ļ�
    static const Key &key(NodePtr x) { return KeyOfValue()(value(x)); }
    // ��ȡ�ڵ� x �Ķ���
    static Color &color(NodePtr x) { return (Color &)(x->color); }

    // ��ȡ��Сֵ
    static NodePtr minimum(NodePtr x) { return Node::minimum(x); }
    // ��ȡ����ֵ
    static NodePtr maximum(NodePtr x) { return Node::maximum(x); }

    // ����
    // x ����ת��, root �Ǹ��ڵ�
    static void RotateLeft(NodePtr x, NodePtr &root);
    // ����
    // x ����ת��, root �Ǹ��ڵ�
    static void RotateRight(NodePtr x, NodePtr &root);
    // ��������ƽ�� (�ı���ɫ����ת)
    // x ���½ڵ�, root �Ǹ��ڵ�
    static void Rebalance(NodePtr x, NodePtr &root);
    // �Ƴ��ڵ����������ƽ��
    // z ��Ҫ�Ƴ��Ľڵ�, root �Ǹ��ڵ�, leftmost ������ڵ�, rightmost
    // �����ҽڵ� �����Ƴ��Ľڵ� (�Ѿ�������)
    static NodePtr RebalanceForErase(NodePtr z, NodePtr &root,
                                     NodePtr &leftmost, NodePtr &rightmost);

public:
    typedef Iterator<Value, Ref, Ptr> iterator;
    typedef Iterator<Value, constRef, constPtr> constIterator;

private:
    // �����Ĳ���ʵ��
    iterator _insert(NodePtr x, NodePtr y, const Value &v);
    // �Ƴ��� x Ϊ���ڵ����������, ���Ҳ�����ƽ�����
    void _erase(NodePtr x);
    // ����һ���ڵ��ֵ����ɫ
    NodePtr _cloneNode(NodePtr x);
    // ������ x Ϊ���ڵ����������һ�����Ľڵ� p ��
    // ���ظ��������ĸ��ڵ� (���ڵ�Ϊ p)
    NodePtr _copy(NodePtr x, NodePtr p);

public:
    // Ĭ�Ϲ��캯��
    RbTree(const Compare &comp = Compare()) : nodeCount(0), keyCompare(comp) {
        _emptyInitialize();
    }
    // ���ƹ��캯��
    RbTree(const RbTree<Key, Value, KeyOfValue, Compare> &t)
        : nodeCount(0), keyCompare(t.keyCompare) {
        if (t.root() == 0) {     // ��� t �ǿ���
            _emptyInitialize();  // ���ʼ��һ�ÿ���
        } else {                 // ��� t �ǿ�������и���
            color(header) = Red;
            root() = _copy(t.root(), header);  // ����
            leftmost() = minimum(root());      // �趨����ڵ�
            rightmost() = maximum(root());     // �趨���ҽڵ�
        }
        nodeCount = t.nodeCount;  // �������Ĵ�С
    }

private:
    // �����ĳ�ʼ��
    void _emptyInitialize() {
        header = new Node();  // ���� header �ڵ�
        color(header) =  // �� header Ϊ��ɫ, �� iterator.operator-- ��
            Red;         // �������ָ��ڵ� (���ڵ�Ϊ��ɫ)
        root() = 0;      // ��ʱû�и��ڵ�
        leftmost() = header;   // �� header �����ӽڵ�Ϊ�Լ�
        rightmost() = header;  // �� header �����ӽڵ�Ϊ�Լ�
    }

public:
    // ��������
    ~RbTree() {
        clear();
        delete header;
    }

    // ���������
    RbTree<Key, Value, KeyOfValue, Compare> &operator=(
        const RbTree<Key, Value, KeyOfValue, Compare> &x);

    // ����ָ����ʼ�ĵ����� (����ڵ�)
    iterator begin() { return leftmost(); }
    constIterator begin() const { return leftmost(); }
    // ����ָ��ĩβ�ĵ�����
    // ��Ϊǰ�պ�, ���Է������ҽڵ�ĺ�һ���ڵ� (�� header)
    iterator end() { return header; }
    constIterator end() const { return header; }

    // �ж����Ƿ�Ϊ��
    bool empty() const { return nodeCount == 0; }
    // �������Ĵ�С (���ڵ�����)
    std::size_t size() const { return nodeCount; }

    // �Ƴ����нڵ�, header �ص���ʼ״̬
    void clear();

    // ������ֵ, �ڵ��ֵ�������ظ�, ���ظ��������Ч
    // ע: ����ֵ�� pair, ��һ��Ԫ����ָ�������ڵ� (���ظ���ֵ�ڵ�) �ĵ�����
    // �ڶ���Ԫ�ر�ʾ�����Ƿ�ɹ�
    std::pair<iterator, bool> insertUnique(const Value &v);
    // ������ֵ, �ڵ��ֵ�����ظ�
    // ����ָ�������ڵ�ĵ�����
    iterator insertEqual(const Value &v);

    // ��ָ��λ�ò�����ֵ, �ڵ��ֵ�������ظ�, ���ظ��������Ч
    // �߼������жϲ���λ���Ƿ���ȷ, ��ȷ��ֱ�Ӳ���
    // ����ʡȥ����λ�õ�ʱ��, ���������Ѱ�ҵ���ȷ��λ���ٲ���
    // ����ָ�������ڵ� (���ظ���ֵ�ڵ�) �ĵ�����
    iterator insertUnique(iterator position, const Value &v);
    // ��ָ��λ�ò�����ֵ, �ڵ��ֵ�����ظ�
    // �߼�ͬ��
    // ����ָ�������ڵ�ĵ�����
    iterator insertEqual(iterator position, const Value &v);

    // ��Χ���� range[first, last) �е�Ԫ��
    // �ڵ��ֵ�������ظ�, ���ظ��������Ч
    template <class InputIterator>
    void insertUnique(InputIterator first, InputIterator last);
    // ��Χ���� range[first, last) �е�Ԫ��, �ڵ��ֵ�����ظ�
    template <class InputIterator>
    void insertEqual(InputIterator first, InputIterator last);

    // �Ƴ�ָ��λ�õĽڵ�
    void erase(iterator position);
    // �Ƴ���ֵΪ k �Ľڵ�
    // �����Ƴ��ڵ������
    std::size_t erase(const Key &k);
    // �Ƴ� [first, last) ��Χ�ڵĽڵ�
    void erase(iterator first, iterator last);
    // �Ƴ���ֵΪ [first, last] ��Χ�ڵĽڵ�
    void erase(const Key *first, const Key *last);

    // ����������������
    // ������ header, nodeCount, keyCompare ����������ʾһ�����ĳ�Ա����
    void swap(RbTree<Key, Value, KeyOfValue, Compare> &t);

    // ���ؼ�ֵΪ k �Ľ��ĸ���
    std::size_t count(const Key &k) const;

    // Ѱ�Ҽ�ֵΪ k �Ľڵ�ĵ�����, �������򷵻� end()
    iterator find(const Key &k);
    constIterator find(const Key &k) const;

    // ���ؼ�ֵΪ k �Ľڵ�����
    // ע: ���� pair, ��һ��Ԫ�����׸� >= k �Ľڵ�ĵ�����
    // �ڶ���Ԫ�����׸� > k �Ľڵ�ĵ�����
    std::pair<iterator, iterator> equalRange(const Key &k);
    std::pair<constIterator, constIterator> equalRange(const Key &k) const;

    // �����׸� >= k �Ľڵ�ĵ�����
    iterator lowerBound(const Key &k);
    constIterator lowerBound(const Key &k) const;

    // �����׸� > k �Ľڵ�ĵ�����
    iterator upperBound(const Key &k);
    constIterator upperBound(const Key &k) const;

    // ���ؽڵ��ļ�ֵ��С�Ƚ�׼�� (��������)
    Compare keyComp() const { return keyCompare; }

    // �жϺ�����Ƿ���ȷ
    bool _rb_verify() const;

private:
    // ���� node �ڵ㵽���ڵ�·���ϵĺ�ɫ�ڵ����
    int _blackCount(NodePtr node, NodePtr root);
};

template <class Key, class Value, class KeyOfValue, class Compare>
typename RbTree<Key, Value, KeyOfValue, Compare>::iterator
RbTree<Key, Value, KeyOfValue, Compare>::_insert(NodePtr x, NodePtr y,
                                                 const Value &v) {
    // x ����ֵ�Ĳ����, y �ǲ����ĸ��ڵ�, v ����ֵ
    NodePtr z;

    // y == header: ��ʱ�ǿ���, ����Ľڵ���Ϊ���ڵ�
    // x != 0
    // v < y: ��ֵС�ڸ��ڵ�
    // ��Ϊ���ӽڵ����
    if (y == header || x != 0 || keyCompare(KeyOfValue()(v), key(y))) {
        z = new Node(v);
        left(y) = z;  // �趨���ڵ�����ӽڵ�
        // ͬʱ�� y == header ʱ, leftmost() = z
        if (y == header) {  // ��� y == header, ˵����ʱֻ��һ�����ڵ�
            root() = z;
            rightmost() = z;           // ����������ҽڵ㶼Ϊ z
        } else if (y == leftmost()) {  // ��� y Ϊ����ڵ�
            leftmost() = z;  // z ��Ϊ y �����ӽڵ�, ��Ϊ�µ� leftmost()
        }
    } else {  // ��Ϊ���ӽڵ����
        z = new Node(v);
        right(y) = z;            // �趨���ڵ�����ӽڵ�
        if (y == rightmost()) {  // ��� y Ϊ���ҽڵ�
            rightmost() = z;  // z ��Ϊ y �����ӽڵ�, ��Ϊ�µ� rightmost()
        }
    }
    parent(z) = y;  // �趨�½ڵ�ĸ��ڵ�
    left(z) = 0;    // �趨�½ڵ�����ӽڵ�
    right(z) = 0;   // �趨�½ڵ�����ӽڵ�

    Rebalance(               // ��������ƽ��
        z, header->parent);  // �½ڵ����ɫ��ƽ��ʱ�趨

    ++nodeCount;         // �ڵ����ۼ�
    return iterator(z);  // ����һ��������ָ�������ڵ�
}

// �Ƴ��� x Ϊ���ڵ����������, ���Ҳ�����ƽ�����
template <class Key, class Value, class KeyOfValue, class Compare>
void RbTree<Key, Value, KeyOfValue, Compare>::_erase(NodePtr x) {
    // ѭ��ֱ���Ƴ���������
    while (x != 0) {
        _erase(right(x));  // �ݹ�
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
    // top ��Ϊ��ǰ�������ĸ��ڵ�
    NodePtr top = _cloneNode(x);
    top->parent = p;
    // ����������
    if (x->right != 0) {
        top->right = _copy(right(x), top);  // �ݹ�
    }
    // �������˵�� top ���������Ѿ�������
    // ����������
    p = top;
    x = left(x);
    while (x != 0) {
        // �������ӽڵ�
        NodePtr y = _cloneNode(x);
        p->left = y;
        y->parent = p;
        // ���Ƶ�ǰ���ӽڵ��µ�������
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
        clear();  // �Ƴ����нڵ�, header �ص���ʼ״̬
        keyCompare = x.keyCompare;
        if (x.root() != 0) {  // ��� x �ǿ�������и���
            root() = _copy(x.root(), header);  // ����
            leftmost() = minimum(root());      // �趨����ڵ�
            rightmost() = maximum(root());     // �趨���ҽڵ�
            nodeCount = x.nodeCount;           // �������Ĵ�С
        }
    }
    return *this;
}

template <class Key, class Value, class KeyOfValue, class Compare>
void RbTree<Key, Value, KeyOfValue, Compare>::clear() {
    if (nodeCount != 0) {      // ������ǿ���
        _erase(root());        // �Ƴ�������
        root() = 0;            // ��ʱû�и��ڵ�
        leftmost() = header;   // �ص� header �ĳ�ʼ״̬
        rightmost() = header;  // �ص� header �ĳ�ʼ״̬
        nodeCount = 0;         // �ڵ���������
    }
}

template <class Key, class Value, class KeyOfValue, class Compare>
std::pair<typename RbTree<Key, Value, KeyOfValue, Compare>::iterator, bool>
RbTree<Key, Value, KeyOfValue, Compare>::insertUnique(const Value &v) {
    NodePtr y = header;  // y ָ�� x �ĸ��ڵ�
    NodePtr x = root();  // �Ӹ��ڵ㿪ʼ
    bool comp = true;
    while (x != 0) {  // ����Ѱ�Һ��ʵĲ����
        y = x;
        comp = keyCompare(KeyOfValue()(v), key(x));
        // v < x ��������, v >= x ��������
        x = comp ? left(x) : right(x);
    }
    iterator j = iterator(y);  // ������� j ָ�����ڵ�ĸ��ڵ�
    if (comp) {                // ��� comp Ϊ true, ��ʾ���������
        if (j == begin()) {  // �������ڵ�ĸ��ڵ�������ڵ� (һֱ <)
            return std::pair<iterator, bool>(
                _insert(x, y, v),  // ˵����ֵ���ظ�
                true);
        } else {  // �����Ϊ����ڵ� (���ֹ� >=), �� --j <= v < j
            --j;  // �游�ڵ���� = v, --j �� j ָ���游�ڵ���������һ���ж�
        }
    }
    // ��������������ж�
    // һ: comp = false, ��ʾ�������Ҳ�, ��ʱ j ��Ϊ���ڵ�, ���ܳ��� j >= v
    // ��: comp = true, ��ʱ j Ϊ�游�ڵ�, ���ܳ��� j >= v
    if (keyCompare(key(j.node),
                   KeyOfValue()(v))) {  // ��� j < v
        return std::pair<iterator, bool>(_insert(x, y, v),
                                         true);  // ˵����ֵ���ظ�
    }
    // ���˴�˵���������֮һ�� j = v ������
    // ����ֵһ�������еļ�ֵ�ظ�, ��������ֵ
    return std::pair<iterator, bool>(j, false);
}

template <class Key, class Value, class KeyOfValue, class Compare>
typename RbTree<Key, Value, KeyOfValue, Compare>::iterator
RbTree<Key, Value, KeyOfValue, Compare>::insertEqual(const Value &v) {
    NodePtr y = header;  // y ָ�� x �ĸ��ڵ�
    NodePtr x = root();  // �Ӹ��ڵ㿪ʼ
    while (x != 0) {     // ����Ѱ�Һ��ʵĲ����
        y = x;
        // v < x ��������, v >= x ��������
        x = keyCompare(KeyOfValue()(v), key(x)) ? left(x) : right(x);
    }
    // x ����ֵ�����, y �ǲ����ĸ��ڵ�, v ����ֵ
    // ע: header �͸��ڵ㻥Ϊ���ڵ�
    return _insert(x, y, v);
}

template <class Key, class Value, class KeyOfValue, class Compare>
typename RbTree<Key, Value, KeyOfValue, Compare>::iterator
RbTree<Key, Value, KeyOfValue, Compare>::insertUnique(iterator position,
                                                      const Value &v) {
    if (position.node == header->left) {       // ����λ���� begin()
        if (size() > 0 &&                      // �ǿ���
            keyCompare(KeyOfValue()(v),        // �� v < begin()
                       key(position.node))) {  // ���ظ���λ����ȷ
            return _insert(position.node, position.node, v);
        } else {  // �ظ���λ�ò���ȷ
            return insertUnique(v).first;
        }
    } else if (position.node == header) {   // ����λ���� end()
        if (keyCompare(key(rightmost()),    // end() < v
                       KeyOfValue()(v))) {  // ���ظ���λ����ȷ
            return _insert(0, rightmost(), v);
        } else {  // �ظ���λ�ò���ȷ
            return insertUnique(v).first;
        }
    } else {  // ����λ��
        iterator before = position;
        --before;
        if (keyCompare(key(before.node), KeyOfValue()(v)) &&
            keyCompare(KeyOfValue()(v),        // before < v < position
                       key(position.node))) {  // ���ظ���λ����ȷ
            if (right(before.node) == 0) {
                return _insert(0, before.node, v);
            } else {
                return _insert(position.node, position.node, v);
            }
        } else {  // �ظ���λ�ò���ȷ
            return insertUnique(v).first;
        }
    }
}

template <class Key, class Value, class KeyOfValue, class Compare>
typename RbTree<Key, Value, KeyOfValue, Compare>::iterator
RbTree<Key, Value, KeyOfValue, Compare>::insertEqual(iterator position,
                                                     const Value &v) {
    if (position.node == header->left) {        // ����λ���� begin()
        if (size() > 0                          // �ǿ���
            && !keyCompare(key(position.node),  // �� begin() >= v
                           KeyOfValue()(v))) {  // λ����ȷ
            return _insert(position.node, position.node, v);
        } else {  // λ�ò���ȷ
            return insertEqual(v);
        }
    } else if (position.node == header) {     // ����λ���� end()
        if (!keyCompare(KeyOfValue()(v),      // v >= end()
                        key(rightmost()))) {  // λ����ȷ
            return _insert(0, rightmost(), v);
        } else {
            return insertEqual(v);
        }
    } else {  // ����λ��
        iterator before = position;
        --before;
        if (!keyCompare(KeyOfValue()(v), key(before.node)) &&
            !keyCompare(key(position.node),  // position >= v >= before
                        KeyOfValue()(v))) {  // λ����ȷ
            if (right(before.node) == 0) {
                return _insert(0, before.node, v);
            } else {
                return _insert(position.node, position.node, v);
            }
        } else {  // λ�ò���ȷ
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
    if (first == begin() && last == end()) {  // �ȼ����Ƴ����нڵ�
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
    NodePtr y = header;  // ����ָ���׸� >= k �Ľڵ�
    NodePtr x = root();  // ��ǰ�ڵ�

    while (x != 0) {                   // �ǿ���
        if (!keyCompare(key(x), k)) {  // k <= x
            y = x, x = left(x);
        } else {  // k > x
            x = right(x);
        }
    }

    iterator j = iterator(y);
    // ��������ڼ�ֵΪ k �Ľڵ��򷵻� end()
    return (j == end() || keyCompare(k, key(j.node))) ? end() : j;
}

template <class Key, class Value, class KeyOfValue, class Compare>
typename RbTree<Key, Value, KeyOfValue, Compare>::constIterator
RbTree<Key, Value, KeyOfValue, Compare>::find(const Key &k) const {
    NodePtr y = header;  // ����ָ���׸� >= k �Ľڵ�
    NodePtr x = root();  // ��ǰ�ڵ�

    while (x != 0) {                   // �ǿ���
        if (!keyCompare(key(x), k)) {  // k <= x
            y = x, x = left(x);
        } else {  // k > x
            x = right(x);
        }
    }

    constIterator j = constIterator(y);
    // ��������ڼ�ֵΪ k �Ľڵ��򷵻� end()
    return (j == end() || keyCompare(k, key(j.node))) ? end() : j;
}

template <class Key, class Value, class KeyOfValue, class Compare>
typename RbTree<Key, Value, KeyOfValue, Compare>::iterator
RbTree<Key, Value, KeyOfValue, Compare>::lowerBound(const Key &k) {
    NodePtr y = header;  // ����ָ���׸� >= k �Ľڵ�
    NodePtr x = root();  // ��ǰ�ڵ�

    while (x != 0) {                   // �ǿ���
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
    NodePtr y = header;  // ����ָ���׸� >= k �Ľڵ�
    NodePtr x = root();  // ��ǰ�ڵ�

    while (x != 0) {                   // �ǿ���
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
    NodePtr y = header;  // ����ָ���׸� > k �Ľڵ�
    NodePtr x = root();  // ��ǰ�ڵ�

    while (x != 0) {                  // �ǿ���
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
    NodePtr y = header;  // ����ָ���׸� > k �Ľڵ�
    NodePtr x = root();  // ��ǰ�ڵ�

    while (x != 0) {                  // �ǿ���
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
    // �ж��Ƿ��ǺϷ��Ŀ���
    if (nodeCount == 0 || begin() == end()) {
        return nodeCount == 0 && begin() == end() && header->left == header &&
               header->right == header;
    }

    int len = _blackCount(leftmost(), root());
    // ѭ�������ڵ��ж��Ƿ�Ϸ�
    for (constIterator it = begin(); it != end(); ++it) {
        NodePtr x = it.node;
        NodePtr L = left(x);
        NodePtr R = right(x);

        // �ж��Ƿ�������������ɫ�ڵ�
        if (x->color == Red) {
            if ((L && L->color == Red) || (R && R->color == Red)) {
                return false;
            }
        }

        // �жϽڵ�λ���Ƿ���ȷ
        if (L && keyCompare(key(x), key(L))) {
            return false;
        }
        if (R && keyCompare(key(R), key(x))) {
            return false;
        }

        // �ж�Ҷ�ӽڵ㵽���ڵ�ĺ�ɫ�ڵ�����Ƿ���ͬ
        if (!L && !R && _blackCount(x, root()) != len) {
            return false;
        }
    }

    // �ж��������ҽڵ��Ƿ���ȷ
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
    NodePtr y = x->right;      // �� y Ϊ��ת������ӽڵ�
    x->right = y->left;        // x �����ӽڵ����Ϊ y �����ӽڵ�
    if (y->left != nullptr) {  // ��� y �����ӽڵ����
        y->left->parent = x;   // ���������ӽڵ�ĸ��ڵ�Ϊ x
    }
    y->parent = x->parent;  // y �ĸ��ڵ����Ϊ x �ĸ��ڵ�

    // �� y ��ȫ��� x (���� x �����丸�ڵ�Ĺ�ϵ)
    if (x == root) {  // ��� x �Ǹ��ڵ� (��ʱ y �ĸ��ڵ��� header)
        root = y;
    } else if (x == x->parent->left) {  // ��� x �Ǹ��ڵ�����ӽڵ�
        x->parent->left = y;            // x ���ڵ�����ӽڵ����Ϊ y
    } else {                            // ��� x �Ǹ��ڵ�����ӽڵ�
        x->parent->right = y;           // x ���ڵ�����ӽڵ����Ϊ y
    }
    y->left = x;
    x->parent = y;
}

template <class Key, class Value, class KeyOfValue, class Compare>
void RbTree<Key, Value, KeyOfValue, Compare>::RotateRight(NodePtr x,
                                                          NodePtr &root) {
    NodePtr y = x->left;        // �� y Ϊ��ת������ӽڵ�
    x->left = y->right;         // x �����ӽڵ����Ϊ y �����ӽڵ�
    if (y->right != nullptr) {  // ��� y �����ӽڵ����
        y->right->parent = x;   // ���������ӽڵ�ĸ��ڵ�Ϊ x
    }
    y->parent = x->parent;  // y �ĸ��ڵ����Ϊ x �ĸ��ڵ�

    // �� y ��ȫ��� x (���� x �����丸�ڵ�Ĺ�ϵ)
    if (x == root) {  // ��� x �Ǹ��ڵ� (��ʱ y �ĸ��ڵ��� header)
        root = y;     // ���ڵ����Ϊ y
    } else if (x == x->parent->right) {  // ��� x �Ǹ��ڵ�����ӽڵ�
        x->parent->right = y;  // x ���ڵ�����ӽڵ����Ϊ y
    } else {                   // ��� x �Ǹ��ڵ�����ӽڵ�
        x->parent->left = y;   // x ���ڵ�����ӽڵ����Ϊ y
    }
    y->right = x;   // y �����ӽڵ����Ϊ x
    x->parent = y;  // x �ĸ��ӽ�����Ϊ y
}

template <class Key, class Value, class KeyOfValue, class Compare>
void RbTree<Key, Value, KeyOfValue, Compare>::Rebalance(NodePtr x,
                                                        NodePtr &root) {
    // �����½ڵ���ɫΪ��ɫ, ��Ϊ�������Ľڵ��Ǻ�ɫ, ��Ȼ�ᵼ������ƽ��
    x->color = Red;
    // �� x �Ǹ��ڵ��� x �ĸ��ڵ�Ϊ��ɫʱ��Ҫ����ƽ�����
    while (x != root && x->parent->color == Red) {
        // һ: ���ڵ����游�ڵ�����ӽڵ�
        if (x->parent == x->parent->parent->left) {
            // �� y Ϊ�����ڵ�
            NodePtr y = x->parent->parent->right;
            // 1: �����ڵ������Ϊ��ɫ
            if (y && y->color == Red) {
                // ���ĸ��ڵ�Ϊ��ɫ
                x->parent->color = Black;
                // ���Ĳ����ڵ�Ϊ��ɫ
                y->color = Black;
                // �����游�ڵ�Ϊ��ɫ
                x->parent->parent->color = Red;
                // �游��ɫ�ɺڱ����ܻ��ƻ��ϲ�ƽ��
                // ����ǰ�ڵ����Ϊ�游�ڵ�������ϼ��
                x = x->parent->parent;
            } else {  // 2: �����ڵ㲻���ڻ�Ϊ��ɫ
                // ʵ������ͬ�����, ��Ϊ nullptr �����������ɫ�ӽڵ�
                // (1): �½ڵ��Ǹ��ڵ�����ӽڵ�
                // ��Ҫ�Ƚ���������Ϊ���ӽڵ�
                if (x == x->parent->right) {
                    x = x->parent;
                    // ����
                    RotateLeft(x, root);
                }
                // (2): �½ڵ��Ǹ��ڵ�����ӽڵ�
                // ���ĸ��ڵ�Ϊ��ɫ
                x->parent->color = Black;
                // �����游�ڵ�Ϊ��ɫf
                x->parent->parent->color = Red;
                // ����
                RotateRight(x->parent->parent, root);
            }
        } else {  // ��: ���ڵ����游�ڵ�����ӽڵ�
            // �� y Ϊ�����ڵ�
            NodePtr y = x->parent->parent->left;
            // 1: �����ڵ������Ϊ��ɫ
            if (y && y->color == Red) {
                // ���ĸ��ڵ�Ϊ��ɫ
                x->parent->color = Black;
                // ���Ĳ����ڵ�Ϊ��ɫ
                y->color = Black;
                // �����游�ڵ�Ϊ��ɫ
                x->parent->parent->color = Red;
                // �游��ɫ�ɺڱ����ܻ��ƻ��ϲ�ƽ��
                // ����ǰ�ڵ����Ϊ�游�ڵ�������ϼ��
                x = x->parent->parent;
            } else {  // 2: �����ڵ㲻���ڻ�Ϊ��ɫ
                // ʵ������ͬ�����, ��Ϊ nullptr �����������ɫ�ӽڵ�
                // (1): �½ڵ��Ǹ��ڵ�����ӽڵ�
                // ��Ҫ�Ƚ���������Ϊ���ӽڵ�
                if (x == x->parent->left) {
                    x = x->parent;
                    // ����
                    RotateRight(x, root);
                }
                // (2): �½ڵ��Ǹ��ڵ�����ӽڵ�
                // ���ĸ��ڵ�Ϊ��ɫ
                x->parent->color = Black;
                // �����游�ڵ�Ϊ��ɫ
                x->parent->parent->color = Red;
                // ����
                RotateLeft(x->parent->parent, root);
            }
        }
    }
    root->color = Black;  // ���ڵ���ԶΪ��ɫ
}

template <class Key, class Value, class KeyOfValue, class Compare>
typename RbTree<Key, Value, KeyOfValue, Compare>::NodePtr
RbTree<Key, Value, KeyOfValue, Compare>::RebalanceForErase(NodePtr z,
                                                           NodePtr &root,
                                                           NodePtr &leftmost,
                                                           NodePtr &rightmost) {
    NodePtr y = z;
    NodePtr x = nullptr;  // x ָ�����ʵ���Ƴ��ڵ���ӽڵ�
    NodePtr xParent = nullptr;

    if (y->left == nullptr) {          // z û�����ӽڵ�
        x = y->right;                  // x ָ�����ӽڵ� (����Ϊ��)
    } else if (y->right == nullptr) {  // z ֻ�����ӽڵ�
        x = y->left;                   // x ָ�����ӽڵ�
    } else {                           // z �������ӽڵ�
        // y ָ���Ƴ��ڵ�ĺ����ڵ�
        y = y->right;
        while (y->left != nullptr) {
            y = y->left;
        }
        x = y->right;  // ָ������ڵ�����ӽڵ� (����Ϊ��)
    }
    if (y != z) {  // z �������ӽڵ�
        // �� y (�����ڵ�) ȡ�� z (�Ƴ��ڵ�) ��λ��
        // ��ͬ���Ƴ� y
        // ͬʱ���� y �����ӽڵ� (�� x) ��λ�� (û�����ӽڵ�)
        z->left->parent = y;  // ȡ�����ӽڵ�
        y->left = z->left;
        if (y != z->right) {  // �������ӽڵ���ȡ�����ӽڵ�
            xParent = y->parent;
            // �������е��� x ��λ��
            if (x) {
                x->parent = y->parent;
            }
            y->parent->left = x;
            y->right = z->right;  // ȡ�����ӽڵ�
            z->right->parent = y;
        } else {  // ����������ӽڵ㲻��Ҫ��ȡ��
            xParent = y;
        }
        // ȡ�����ڵ�
        if (root == z) {  // ����Ƴ����Ǹ��ڵ�
            root = y;     // y ����Ҫ�̳и��ڵ������λ��
        } else if (z->parent->left == z) {
            z->parent->left = y;
        } else {
            z->parent->right = y;
        }
        y->parent = z->parent;
        std::swap(y->color, z->color);  // ������ɫ
        y = z;                          // y ����ָ����Ҫ�Ƴ��Ľڵ�
    } else {  // z ֻ��һ���ӽ�� (�� x), �� x ȡ�� z ��λ��
        xParent = y->parent;
        if (x) {
            x->parent = y->parent;
        }
        if (root == z) {  // ����Ƴ����Ǹ��ڵ�
            root = x;     // x ����Ҫ�̳и��ڵ������λ��
        } else if (z->parent->left == z) {
            z->parent->left = x;
        } else {
            z->parent->right = x;
        }
        if (leftmost == z) {  // ����Ƴ���������ڵ�, ά�� leftmost ��λ��
            if (z->right == nullptr) {
                leftmost = z->parent;
            } else {
                leftmost = Node::minimum(x);
            }
        }
        if (rightmost == z) {  // ����Ƴ���������ڵ�, ά�� rightmost ��λ��
            if (z->left == nullptr) {
                rightmost = z->parent;
            } else {
                rightmost = Node::maximum(x);
            }
        }
    }
    // ����ƽ��
    // ɾ����ɫ�ڵ㲻���ƻ�ƽ��
    if (y->color != Red) {
        // x �Ǹ��ڵ㲢�� x Ϊ�ջ��ɫ, ����һ����ɫ�ڵ�
        while (x != root && (x == nullptr || x->color == Black)) {
            if (x == xParent->left) {        // �Ƴ��������ӽڵ�
                NodePtr w = xParent->right;  // w ָ�� x ���ֵܽڵ�
                // һ: �ֵܽڵ�Ϊ��ɫ
                // 1.���ֵܽڵ��Ϊ��ɫ
                // 2.�����ڵ��Ϊ��ɫ
                // 3.�Ը��ڵ��������
                // 4.�������������� x ���ֵܽڵ�
                // ��ʱ x ������һ����ɫ�ڵ�, ������һ��״̬��������
                if (w->color == Red) {
                    w->color = Black;
                    xParent->color = Red;
                    RotateLeft(xParent, root);
                    w = xParent->right;
                }
                // ��: �ֵܽڵ�Ϊ��ɫ, �ֵܽڵ�������ӽڵ�Ҳ��Ϊ��ɫ
                // 1.���ֵܽڵ��Ϊ��ɫ
                // 2.���� x �ĸ��ڵ�Ϊ�µ� x �ڵ�
                // ��ʱ x �ĸ��ڵ�������һ����ɫ�ڵ�, ��Ϊ�µ� x �ڵ�
                if ((w->left == nullptr || w->left->color == Black) &&
                    (w->right == nullptr || w->right->color == Black)) {
                    w->color = Red;
                    x = xParent;
                    xParent = xParent->parent;
                } else {
                    // ��: �ֵܽڵ�Ϊ��ɫ, �����ӽڵ�Ϊ��ɫ, ���ӽڵ�Ϊ��ɫ
                    // 1.���ֵܽڵ�����ӽڵ��Ϊ��ɫ
                    // 2.���ֵܽڵ��Ϊ��ɫ
                    // 3.���ֵܽڵ��������
                    // 4.�������������� x ���ֵܽڵ�
                    // ��ʱ���������
                    if (w->right == nullptr || w->right->color == Black) {
                        if (w->left) {
                            w->left->color = Black;
                        }
                        w->color = Red;
                        RotateRight(w, root);
                        w = xParent->right;
                    }
                    // ��: �ֵܽڵ�Ϊ��ɫ, �����ӽڵ�Ϊ��ɫ
                    // 1.���ֵܽڵ��Ϊ���ڵ����ɫ
                    // 2.�����ڵ��Ϊ��ɫ
                    // 3.���ֵܽڵ�����ӽڵ��Ϊ��ɫ
                    // 4.�Ը��ڵ��������
                    // ���ƽ��, �˳�ѭ��
                    w->color = xParent->color;
                    xParent->color = Black;
                    if (w->right) {
                        w->right->color = Black;
                    }
                    RotateLeft(xParent, root);
                    break;
                }
            } else {  // �Ƴ��������ӽڵ�, �������ƽ����̶�Ӧ
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