#include <queue>
#include <iostream>

namespace COP3530 {
    template<typename K, typename V>
    class RBST {
    private:

        struct Node {
            int left;
            int right;
            K key;
            V val;

            Node() {
                left = -1;
                right = -1;
            }

            Node(int left) {
                this->left = left;
                right = -1;
            }

            void set(K key, V value) {
                left = -1;
                this->key = key;
                this->val = value;
            }

        };

        int root;
        int free;
        int used;
        Node* tree;
        std::size_t m;

        void insert_as_leaf(int &roo, K key, V val, int &count) {
            if (key < tree[roo].key) {
                if (tree[roo].left != -1) {
                    ++count;
                    insert_as_leaf(tree[roo].left, key, val, count);
                }
                else {
                    int left = tree[free].left;
                    tree[free].set(key, val);
                    tree[roo].left = free;
                    free = left;
                }
            }
            else {
                if (tree[roo].right != -1) {
                    ++count;
                    insert_as_leaf(tree[roo].right, key, val, count);
                }
                else {
                    int left = tree[free].left;
                    tree[free].set(key, val);
                    tree[roo].right = free;
                    free = left;
                }
            }
        }

        void insert_as_root(int &roo, K key, V val, int &count) {
            if (roo == -1) {
                int left = tree[free].left;
                tree[free].set(key, val);
                roo = free;
                free = left;
                return;
            }
            if (key < tree[roo].key) {
                ++count;
                insert_as_root(tree[roo].left, key, val, count);
                rotate_right(roo);
            }
            else {
                ++count;
                insert_as_root(tree[roo].right, key, val, count);
                rotate_left(roo);
            }
        }

        void rotate_right(int &roo) {
            int temp = tree[roo].left;
            tree[roo].left = tree[temp].right;
            tree[temp].right = roo;
            roo = temp;
        }

        void rotate_left(int &roo) {
            int temp = tree[roo].right;
            tree[roo].right = tree[temp].left;
            tree[temp].left = roo;
            roo = temp;
        }

        int calcHeight(int node) {
            if (node == -1)
                return 0;
            int a = 1 + calcHeight(tree[node].left);
            int b = 1 + calcHeight(tree[node].right);
            return (a > b) ? a : b;
        }

    public:

        RBST(std::size_t m) {
            this->m = m;
            tree = new Node[m];
            free = 0;
            root = -1;
            for (int i = 0; i < m; ++i) {
                tree[i] = *(new Node(i + 1));
            }
            tree[m - 1].left = -1;
            used = 0;
        }

        int search(K key, V& value) {
            int probe = -1;
            int i = root;
            while (i != -1) {
                ++probe;
                if (key == tree[i].key) {
                    value = tree[i].val;
                    return probe;
                }
                else if (key < tree[i].key)
                    i = tree[i].left;
                else
                    i = tree[i].right;
            }
            if (probe < 0)
                return probe;
            return -1 * probe;
        }

        int insert(K key, V value) {
            if (used == m)
                return -1;
            int probe = 0;
            int c;
            (used == 0) ? c = 0 : c = rand() % used;
            (c == 0) ? insert_as_root(root, key, value, probe)
                    : insert_as_leaf(root, key, value, probe);
            ++used;
            return probe;
        }

        int remove(K key, V& value) {
            int probe = 0;
            if (used == 0)
                return -1;
            int head = root;
            int parent = -2;
            while (tree[head].key != key) {
                parent = head;
                if (head == -1)
                    return -1 * probe;
                if (key > tree[head].key)
                    head = tree[head].right;
                else
                    head = tree[head].left;
                ++probe;
            }
            if (parent == -2) {  // Removing from the root
                if (tree[head].left == -1 && tree[head].right == -1) { // Last node
                    root = -1;
                }
                else if (tree[head].left != -1 && tree[head].right == -1) { // Left child
                    root = tree[head].left;
                }
                else if (tree[head].left == -1 && tree[head].right != -1) { // Right child
                    root = tree[head].right;
                }
                else {
                    root = tree[head].left;
                    int lChild = tree[head].left;
                    int lParent = head;
                    while (tree[lChild].right != -1) {
                        lParent = lChild;
                        lChild = tree[lChild].right;
                    }
                    if (head == tree[parent].right)
                        tree[parent].right = lChild;
                    else
                        tree[parent].left = lChild;
                    if (lParent != head) {
                        tree[lChild].left = tree[head].left;
                        tree[lParent].right = -1;
                    }
                    else
                        tree[lChild].left = -1;
                    tree[lChild].right = tree[head].right;
                }
            }
            else if (tree[head].left == -1 && tree[head].right == -1) { // Leaf Node! Jackpot!
                if (head == tree[parent].left)
                    tree[parent].left = -1;
                else
                    tree[parent].right = -1;
            }
            else if (tree[head].left == -1 && tree[head].right != -1) { // One child
                if (head == tree[parent].left)
                    tree[parent].left = tree[head].right;
                else
                    tree[parent].right = tree[head].right;
            }
            else if (tree[head].right == -1 && tree[head].left != -1) {
                if (head == tree[parent].left)
                    tree[parent].left = tree[head].left;
                else
                    tree[parent].right = tree[head].left;
            }
            else { // Two children
                int lChild = tree[head].left;
                int lParent = head;
                while (tree[lChild].right != -1) {
                    lParent = lChild;
                    lChild = tree[lChild].right;
                }
                if (head == tree[parent].right)
                    tree[parent].right = lChild;
                else
                    tree[parent].left = lChild;
                if (lParent != head) {
                    tree[lChild].left = tree[head].left;
                    tree[lParent].right = -1;
                }
                else
                    tree[lChild].left = -1;
                tree[lChild].right = tree[head].right;
            }
            value = tree[head].val;
            tree[head].left = free;
            tree[head].right = -1;
            free = head;
            --used;
            return probe;

        }

        void clear() {
            delete tree;
            tree = new Node[m];
            free = 0;
            root = -1;
            for (int i = 0; i < m; ++i) {
                tree[i] = *(new Node(i + 1));
            }
            tree[m - 1].left = -1;
            used = 0;
        }

        bool is_empty() {
            return used == 0;
        }

        std::size_t capacity() {
            return m;
        }

        std::size_t size() {
            return used;
        }

        double load() {
            return used / m;
        }

        RBST<int, int> cluster_distribution() {
            COP3530::RBST<int, int> myCluster(m / 4);
            std::queue<int> levelOrder;
            int nav = root;
            levelOrder.push(nav);
            while (!levelOrder.empty()) {
                int next = levelOrder.front();
                if (tree[next].left != -1)
                    levelOrder.push(tree[next].left);
                if (tree[next].right != -1)
                    levelOrder.push(tree[next].right);
                else if (tree[next].left == -1 && tree[next].right == -1) {
                    V v;
                    int val;
                    int length = search(tree[next].key, v);
                    if (myCluster.search(length, val) > -1)
                        myCluster.insert(length, ++val);
                    else
                        myCluster.insert(length, 1);
                }
            }
            return myCluster;
        }

        Key 

        void print(std::ostream& out) {
            std::queue<int> levelOrder;
            int nav = root;
            levelOrder.push(nav);
            int a = (int)pow(2, calcHeight(root)) - 1;
            for (int i = 0; i < a; ++i) {
                int next = levelOrder.front();
                int nLeft = tree[next].left;
                int nRight = tree[next].right;
                levelOrder.pop();
                if (next == -1) {
                    out << "-,";
                    levelOrder.push(-1);
                    levelOrder.push(-1);
                }
                else {
                    levelOrder.push(nLeft);
                    levelOrder.push(nRight);

                    out << tree[next].key << ",";
                }
            }
            out << std::endl;
        }
    };
}