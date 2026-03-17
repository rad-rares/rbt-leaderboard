/*
You are maintaining a live leaderboard for an online game. Players gain and lose points over time. You must efficiently
update scores and answer ranking queries.
Operations
You will receive QQQ operations of the following types:
ADD player score
 Add a new player with the given score.


UPDATE player delta
 Increase or decrease the player's score by delta.


REMOVE player
 Remove the player from the leaderboard.


TOP k
 Return the k players with the highest scores.



Input Example
ADD Alice 120
ADD Bob 90
ADD Carol 150
UPDATE Bob 50
TOP 2
REMOVE Carol
TOP 2
Expected Output
Carol 150
Bob 140

Bob 140
Alice 120

 */

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>


enum Color { RED, BLACK };

struct Node {
    int         score;
    std::string name;
    Color       color;
    Node*       left;
    Node*       right;
    Node*       parent;
};

Node* NIL  = nullptr;
Node* root = nullptr;

std::unordered_map<std::string, int> scoreMap;

void initTree() {
    NIL   = new Node{0, "", BLACK, nullptr, nullptr, nullptr};
    root  = NIL;
}

void destroyHelper(Node* x) {
    if (x == NIL) return;
    destroyHelper(x->left);
    destroyHelper(x->right);
    delete x;
}

void destroyTree() {
    destroyHelper(root);
    delete NIL;
    NIL  = nullptr;
    root = nullptr;
}



int cmp(int scoreA, const std::string& nameA,
        int scoreB, const std::string& nameB) {
    if (scoreA != scoreB) return scoreA - scoreB;
    return nameA.compare(nameB);
}



void rotateLeft(Node* x) {
    Node* y  = x->right;
    x->right = y->left;
    if (y->left != NIL) y->left->parent = x;

    y->parent = x->parent;
    if      (x->parent == NIL)         root             = y;
    else if (x == x->parent->left)     x->parent->left  = y;
    else                               x->parent->right = y;

    y->left   = x;
    x->parent = y;
}

void rotateRight(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    if (y->right != NIL) y->right->parent = x;

    y->parent = x->parent;
    if      (x->parent == NIL)         root             = y;
    else if (x == x->parent->right)    x->parent->right = y;
    else                               x->parent->left  = y;

    y->right  = x;
    x->parent = y;
}



void transplant(Node* u, Node* v) {
    if      (u->parent == NIL)         root             = v;
    else if (u == u->parent->left)     u->parent->left  = v;
    else                               u->parent->right = v;
    v->parent = u->parent;
}



Node* subtreeMax(Node* x) {
    while (x->right != NIL) x = x->right;
    return x;
}

Node* subtreeMin(Node* x) {
    while (x->left != NIL) x = x->left;
    return x;
}

Node* findNode(int score, const std::string& name) {
    Node* x = root;
    while (x != NIL) {
        int c = cmp(score, name, x->score, x->name);
        if      (c < 0) x = x->left;
        else if (c > 0) x = x->right;
        else            return x;
    }
    return NIL;
}


void insertFixup(Node* z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Node* uncle = z->parent->parent->right;
            if (uncle->color == RED) {
                z->parent->color         = BLACK;
                uncle->color             = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) { z = z->parent; rotateLeft(z); }
                z->parent->color         = BLACK;
                z->parent->parent->color = RED;
                rotateRight(z->parent->parent);
            }
        } else {
            Node* uncle = z->parent->parent->left;
            if (uncle->color == RED) {
                z->parent->color         = BLACK;
                uncle->color             = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) { z = z->parent; rotateRight(z); }
                z->parent->color         = BLACK;
                z->parent->parent->color = RED;
                rotateLeft(z->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

void insertNode(int score, const std::string& name) {
    Node* parent = NIL;
    Node* curr   = root;

    while (curr != NIL) {
        parent = curr;
        int c = cmp(score, name, curr->score, curr->name);
        if      (c < 0) curr = curr->left;
        else if (c > 0) curr = curr->right;
        else            return;   // already exists
    }

    Node* z = new Node{score, name, RED, NIL, NIL, parent};

    if      (parent == NIL)                                  root          = z;
    else if (cmp(score, name, parent->score, parent->name) < 0)
                                                             parent->left  = z;
    else                                                     parent->right = z;

    if (z->parent == NIL)         { root->color = BLACK; return; }
    if (z->parent->parent == NIL) return;

    insertFixup(z);
}


void deleteFixup(Node* x) {
    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            Node* sib = x->parent->right;
            if (sib->color == RED) {
                sib->color = BLACK; x->parent->color = RED;
                rotateLeft(x->parent); sib = x->parent->right;
            }
            if (sib->left->color == BLACK && sib->right->color == BLACK) {
                sib->color = RED; x = x->parent;
            } else {
                if (sib->right->color == BLACK) {
                    sib->left->color = BLACK; sib->color = RED;
                    rotateRight(sib); sib = x->parent->right;
                }
                sib->color = x->parent->color;
                x->parent->color = BLACK; sib->right->color = BLACK;
                rotateLeft(x->parent); x = root;
            }
        } else {
            Node* sib = x->parent->left;
            if (sib->color == RED) {
                sib->color = BLACK; x->parent->color = RED;
                rotateRight(x->parent); sib = x->parent->left;
            }
            if (sib->right->color == BLACK && sib->left->color == BLACK) {
                sib->color = RED; x = x->parent;
            } else {
                if (sib->left->color == BLACK) {
                    sib->right->color = BLACK; sib->color = RED;
                    rotateLeft(sib); sib = x->parent->left;
                }
                sib->color = x->parent->color;
                x->parent->color = BLACK; sib->left->color = BLACK;
                rotateRight(x->parent); x = root;
            }
        }
    }
    x->color = BLACK;
}

void deleteNode(int score, const std::string& name) {
    Node* z = findNode(score, name);
    if (z == NIL) return;

    Node* y              = z;
    Color yOriginalColor = y->color;
    Node* x;

    if (z->left == NIL) {
        x = z->right; transplant(z, z->right);
    } else if (z->right == NIL) {
        x = z->left; transplant(z, z->left);
    } else {
        y              = subtreeMin(z->right);
        yOriginalColor = y->color;
        x              = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            transplant(y, y->right);
            y->right         = z->right;
            y->right->parent = y;
        }
        transplant(z, y);
        y->left         = z->left;
        y->left->parent = y;
        y->color        = z->color;
    }

    delete z;
    if (yOriginalColor == BLACK) deleteFixup(x);
}

// leaderboard

void cmdAdd(const std::string& name, int score) {
    if (scoreMap.count(name)) {
        std::cout << name << " already exists.\n";
        return;
    }
    scoreMap[name] = score;
    insertNode(score, name);
}

void cmdUpdate(const std::string& name, int delta) {
    auto it = scoreMap.find(name);
    if (it == scoreMap.end()) {
        std::cout << name << " not found.\n";
        return;
    }
    int oldScore = it->second;
    int newScore = oldScore + delta;

    deleteNode(oldScore, name);
    scoreMap[name] = newScore;
    insertNode(newScore, name);
}

void cmdRemove(const std::string& name) {
    auto it = scoreMap.find(name);
    if (it == scoreMap.end()) {
        std::cout << name << " not found.\n";
        return;
    }
    deleteNode(it->second, name);
    scoreMap.erase(it);
}

void collectTop(Node* x, int k, int& found) {
    if (x == NIL || found >= k) return;
    collectTop(x->right, k, found);       // visit higher scores first
    if (found < k) {
        std::cout << x->name << " " << x->score << "\n";
        ++found;
    }
    collectTop(x->left, k, found);
}

void cmdTop(int k) {
    if (root == NIL) { std::cout << "(leaderboard is empty)\n"; return; }
    int found = 0;
    collectTop(root, k, found);
}


int main() {
    initTree();

    std::cout << "Leaderboard ready. Enter operations (blank line to finish):\n"
              << "  ADD <player> <score>\n"
              << "  UPDATE <player> <delta>\n"
              << "  REMOVE <player>\n"
              << "  TOP <k>\n\n";

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) break;

        std::istringstream ss(line);
        std::string op;
        ss >> op;

        if (op == "ADD") {
            std::string name; int score;
            ss >> name >> score;
            cmdAdd(name, score);

        } else if (op == "UPDATE") {
            std::string name; int delta;
            ss >> name >> delta;
            cmdUpdate(name, delta);

        } else if (op == "REMOVE") {
            std::string name;
            ss >> name;
            cmdRemove(name);

        } else if (op == "TOP") {
            int k;
            ss >> k;
            cmdTop(k);

        } else {
            std::cout << "Unknown operation: " << op << "\n";
        }
    }

    destroyTree();
    return 0;
}