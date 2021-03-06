// Red Black Tree
// Based on wikipedia's implementation 
// http://en.wikipedia.org/wiki/Red%E2%80%93black_tree
template<typename T>
struct RBTree {
    enum Color {
        RED,
        BLACK
    };
    struct Node {
        T value;
        Node *left, *right;
        Node *parent;
        Color color;

        Node(const T &value, Node *parent) : value(value), left(nullptr), right(nullptr), parent(parent), color(RED) {}
        ~Node() {
            delete left;
            delete right;
        }

        Node* sibiling() const {
            return parent->right == this ? parent->left : parent->right;
        }

        Node* uncle() const {
            return parent->sibiling();
        }
    };

    Node *root;

    RBTree() : root(nullptr) {}
    ~RBTree() {
        delete root;
    }

    void insert(const T &t) {
        Node **cur = &root;
        Node *parent = nullptr;
        while(*cur) {
            parent = *cur;
            if((*cur)->value == t) return; // Already exists
            if(t < (*cur)->value) {
                cur = &(*cur)->left;
            } else {
                cur = &(*cur)->right;
            }
        }
        *cur = new Node(t, parent);
        balance(*cur);
    }

    bool find(const T &t) const {
        const Node *cur = root;
        while(cur) {
            if(cur->value == t) return true;
            if(t < cur->value) {
                cur = cur->left;
            } else {
                cur = cur->right;
            }
        }
        return false;
    }

private:
    void balance(Node *cur) {
        if(cur == root) { // Case 1: This is root
            cur->color = BLACK;
            return;
        }
        if(cur->parent->color == BLACK) { // Case 2: Parent is black
            // The tree is still valid
            return;
        }
        auto uncle = cur->uncle();
        auto grand = cur->parent->parent;
        if(uncle && uncle->color == RED) { // Case 3: Both parent and uncle are red
            // Flip colors of parent, grandparent and uncle
            cur->parent->color = BLACK;
            grand->color = RED;
            uncle->color = BLACK;
            // Now grandparent may violate tree property
            balance(grand);
            return;
        }
        if(uncle == nullptr || uncle->color == BLACK) { // Case 4 & 5: Parent is red whereas uncle is black
             // Case 4: Parent and cur are opposite children
            if(cur == cur->parent->right && cur->parent == grand->left) {
                rotate_left(cur->parent);
                cur = cur->left;
            } else if(cur == cur->parent->left && cur->parent == grand->right) {
                rotate_right(cur->parent);
                cur = cur->right;
            }

            // Now cur and parent are surely in same direction
            // Then perform rotation on grandparent
            grand->color = RED;
            cur->parent->color = BLACK;
            if(cur == cur->parent->left) {
                rotate_right(grand);
            } else {
                rotate_left(grand);
            }
        }
    }

    void rotate_left(Node *n) {
        auto parent = n->parent;
        auto nrl = n->right->left;
        auto r = n->right;
        if(parent) {
            if(parent->left == n) {
                parent->left = r;
            } else if(parent->right == n) {
                parent->right = r;
            }
        } else {
            root = r;
        }
        r->parent = parent;
        r->left = n;
        n->right = nrl;
        n->parent = r;
        if(nrl) nrl->parent = n;
    }

    void rotate_right(Node *n) {
        auto parent = n->parent;
        auto nlr = n->left->right;
        auto l = n->left;
        if(parent) {
            if(parent->left == n) {
                parent->left = l;
            } else if(parent->right == n) {
                parent->right = l;
            }
        } else {
            root = l;
        }
        l->parent = parent;
        l->right = n;
        n->left = nlr;
        n->parent = l;
        if(nlr) nlr->parent = n;
    }
};
// Used in
// AOJ(ALDS1_4_C)

// Red Black Tree with range sum
// References:
//   * http://en.wikipedia.org/wiki/Red%E2%80%93black_tree
//   * Introduction to Algorithms Second Edition (Chapter 13. Red-Black Trees)
template<typename T>
struct RBTree {
    enum Color {
        RED,
        BLACK
    };
    struct Node {
        T value, sum;
        int size;
        Node *children[2];
        Node *parent;
        Color color;

        Node(const T &value, Node *parent) : value(value), sum(value), size(1), children{nullptr,nullptr}, parent(parent), color(RED) {}
        ~Node() {
            delete children[0];
            delete children[1];
        }

        int dir() const {
            return parent->children[0] == this ? 0 : 1;
        }

        Node* sibiling() const {
            return parent->children[1] == this ? parent->children[0] : parent->children[1];
        }

        Node* uncle() const {
            return parent->sibiling();
        }
    };

    Node *root;

    RBTree() : root(nullptr) {}
    ~RBTree() {
        delete root;
    }

    void insert(const T &t) {
        Node **cur = &root;
        Node *parent = nullptr;
        while(*cur) {
            parent = *cur;
            if(t < (*cur)->value) {
                cur = &(*cur)->children[0];
            } else {
                cur = &(*cur)->children[1];
            }
        }
        *cur = new Node(t, parent);
        Node *n = parent;
        while(n) {
            n->sum += t;
            n->size++;
            n = n->parent;
        }
        balance(*cur);
    }

    void erase(const T &t) {
        Node *cur = root;
        while(cur) {
            if(cur->value == t) break;
            if(t < cur->value) {
                cur = cur->children[0];
            } else {
                cur = cur->children[1];
            }
        }
        if(cur == nullptr) return;

        Node *y = cur;
        if(cur->children[0] != nullptr && cur->children[1] != nullptr) {
            y = cur->children[1];
            while(y->children[0]) y = y->children[0];
        }

        // Delete y
        Node *x = y->children[0] ? y->children[0] : y->children[1];
        const int ydir = (y == root ? -1 : y->dir());
        if(x) x->parent = y->parent;
        if(y == root) {
            root = x;
        } else {
            y->parent->children[y->dir()] = x;
        }

        // Copy y's data into cur
        Node *yy = y->parent;
        if(y != cur) {
            while(yy != cur) {
                yy->sum -= y->value;
                yy->size--;
                yy = yy->parent;
            }
        }
        const T v = cur->value;
        cur->value = y->value;
        while(yy) {
            yy->sum -= v;
            yy->size--;
            yy = yy->parent;
        }
        if(y->color == BLACK) {
            balance_erase(y->parent, ydir);
            if(root) root->color = BLACK;
            y->children[0] = y->children[1] = nullptr;
        }
        delete y;
    }

    bool find(const T &t) const {
        const Node *cur = root;
        while(cur) {
            if(cur->value == t) return true;
            if(t < cur->value) {
                cur = cur->children[0];
            } else {
                cur = cur->children[1];
            }
        }
        return false;
    }

    T minsum(int K) const {
        return minsum_inner(root, K);
    }

    void print_tree() const {
        print_tree(root);
        cout << endl;
    }
    void print_tree(Node *n) const {
        if(n) {
            cout << "([" << n->value << "," << n->sum << "," << n->size << "," << (n->color == BLACK ? 'B' : 'R') << "] ";
            print_tree(n->children[0]);
            print_tree(n->children[1]);
            cout << ")";
        } else {
            cout << "(null)";
        }
    }

private:
    T minsum_inner(Node *n, int &K) const {
        if(n == nullptr) return 0;
        if(n->size <= K) {
            K -= n->size;
            return n->sum;
        } else {
            T sum = 0;
            if(n->children[0]) {
                sum += minsum_inner(n->children[0], K);
            }
            if(K > 0) {
                sum += n->value;
                --K;
            }
            if(K > 0) {
                assert(n->children[1]);
                sum += minsum_inner(n->children[1], K);
            }
            return sum;
        }
    }

    void balance(Node *cur) {
        if(cur == root) { // Case 1: This is root
            cur->color = BLACK;
            return;
        }
        if(cur->parent->color == BLACK) { // Case 2: Parent is black
            // The tree is still valid
            return;
        }
        auto uncle = cur->uncle();
        auto grand = cur->parent->parent;
        if(uncle && uncle->color == RED) { // Case 3: Both parent and uncle are red
            // Flip colors of parent, grandparent and uncle
            cur->parent->color = BLACK;
            grand->color = RED;
            uncle->color = BLACK;
            // Now grandparent may violate tree property
            balance(grand);
            return;
        }
        if(uncle == nullptr || uncle->color == BLACK) { // Case 4 & 5: Parent is red whereas uncle is black
            // Case 4: Parent and cur are opposite children
            if(cur->dir() != cur->parent->dir()) {
                const int d = cur->parent->dir();
                rotate(cur->parent, cur->parent->dir());
                cur = cur->children[d];
            }
            
            // Now cur and parent are surely in same direction
            // Then perform rotation on grandparent
            grand->color = RED;
            cur->parent->color = BLACK;
            rotate(grand, 1-cur->dir());
        }
    }

    void balance_erase(Node *cur, int dir) {
        // Constraint: cur->children[dir] is double-blacked
        while(cur != nullptr && is_black(cur->children[dir])) {
            // Note that w must exist because cur->children[dir] is BLACK.
            // If w is sentinel, double-blacked cur->children[dir] has greater black depth than w.
            // This violates constraint.
            Node *w = cur->children[1-dir];
            assert(w);
            if(w->color == RED) { // Case 1: w is RED
                w->color = BLACK;
                cur->color = RED;
                rotate(cur, dir);
                w = cur->children[1-dir];
            }
            if(is_black(w->children[0]) && is_black(w->children[1])) { // Case 2: w's chidren are both black
                w->color = RED;
                if(cur->parent) dir = cur->dir();
                cur = cur->parent;
            } else {
                if(is_black(w->children[1-dir])) { // Case 3: w's opposite child is black
                    w->children[dir]->color = BLACK;
                    w->color = RED;
                    rotate(w, 1-dir);
                    w = cur->children[1-dir];
                }
                // Case 4: Now it's sure that w's opposite child is red
                w->color = cur->color;
                cur->color = BLACK;
                w->children[1-dir]->color = BLACK;
                rotate(cur, dir);
                root->color = BLACK;
                return;
            }
        }
        if(cur && cur->children[dir]) cur->children[dir]->color = BLACK;
    }

    void rotate(Node *n, int dir) {
        auto parent = n->parent;
        auto c = n->children[1-dir];
        assert(c);
        auto gc = c->children[dir];
        if(parent) {
            if(parent->children[0] == n) {
                parent->children[0] = c;
            } else {
                parent->children[1] = c;
            }
        } else {
            root = c;
        }
        n->children[1-dir] = gc;
        n->parent = c;
        n->sum -= c->sum;
        n->size -= c->size;
        if(gc) {
            gc->parent = n;
            n->sum += gc->sum;
            n->size += gc->size;
            c->sum -= gc->sum;
            c->size -= gc->size;
        }
        c->parent = parent;
        c->children[dir] = n;
        c->sum += n->sum;
        c->size += n->size;
    }

    bool is_black(Node *n) const {
        return n == nullptr || n->color == BLACK;
    }
};
// Used in:
// AtCoder(Donuts2015 D)

