#include <iostream>
#include <regex>
#include <queue>


using namespace std;

class AVLTree {
private:
    struct Node {
        string name;
        int gatorId;
        int height;
        Node* left;
        Node* right;

        Node(const string& name, int gatorId)
                : name(name), gatorId(gatorId), height(1), left(nullptr), right(nullptr) {}
    };

    Node* root;
    bool found, duplicate; // Flag to track if the first ID with associated name is found

    // Helper functions
    string Preorder(Node* node) {
        if (node == nullptr)
            return "";

        string result = node->name;

        string leftSubtree = Preorder(node->left);
        string rightSubtree = Preorder(node->right);

        // Concatenate the left subtree
        if (!leftSubtree.empty())
            result += ", " + leftSubtree;

        // Concatenate the right subtree
        if (!rightSubtree.empty())
            result += ", " + rightSubtree;

        return result;
    }

    string Inorder(Node* node) {
        if (node == nullptr)
            return "";

        string leftSubtree = Inorder(node->left);
        string rightSubtree = Inorder(node->right);
        string result = "";

        // Concatenate the left subtree
        if (!leftSubtree.empty())
            result += leftSubtree + ", ";

        // Concatenate the current node
        result += node->name;

        // Concatenate the right subtree
        if (!rightSubtree.empty())
            result += ", " + rightSubtree;

        return result;
    }

    string Postorder(Node* node) {
        if (node == nullptr)
            return "";

        string leftSubtree = Postorder(node->left);
        string rightSubtree = Postorder(node->right);
        string result = "";

        // Concatenate the left subtree
        if (!leftSubtree.empty())
            result += leftSubtree + ", ";

        // Concatenate the right subtree
        if (!rightSubtree.empty())
            result += rightSubtree + ", ";

        // Concatenate the current node
        result += node->name;

        return result;
    }

    int getLevelCount(Node* node) {
        if (node == nullptr)
            return 0;

        queue<Node*> q;
        q.push(node);

        int levelCount = 0;
        while (!q.empty()) {
            int levelSize = q.size();

            // Process nodes at the current level
            while (levelSize > 0) {
                Node* curr = q.front();
                q.pop();
                levelSize--;

                // Add child nodes to the queue
                if (curr->left != nullptr)
                    q.push(curr->left);
                if (curr->right != nullptr)
                    q.push(curr->right);
            }

            levelCount++;
        }

        return levelCount;
    }

    int getHeight(Node* node) {
        if (node == nullptr)
            return 0;
        return node->height;
    }

    Node* rotateLeft(Node *node)
    {
        Node* newNode = node->right;
        node->right = newNode->left;
        newNode->left = node;
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        newNode->height = 1 + max(getHeight(newNode->left), getHeight(newNode->right));
        return newNode;
    }

    Node* rotateRight(Node *node)
    {
        Node* newNode = node->left;
        node->left = newNode->right;
        newNode->right = node;
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        newNode->height = 1 + max(getHeight(newNode->left), getHeight(newNode->right));
        return newNode;
    }


    Node* insertNode(Node* node, const string& name, int gatorId) {
        if (node == nullptr)
            return new Node(name, gatorId);

        if (gatorId < node->gatorId)
            node->left = insertNode(node->left, name, gatorId);
        else if (gatorId > node->gatorId)
            node->right = insertNode(node->right, name, gatorId);
        else{
            duplicate = true;
            return node;
        }


        // Update the height of the current node
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));

        // Perform rotations if necessary to balance the tree
        int balance = getHeight(node->left) - getHeight(node->right);
        if (balance > 1) {
            if (gatorId < node->left->gatorId) {
                return rotateRight(node);
            }
            else {
                node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
        }
        if (balance < -1) {
            if (gatorId > node->right->gatorId) {
                return rotateLeft(node);
            }
            else {
                node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
        }

        return node;
    }


    string searchById(Node* node, int gatorId) {
        if (node == nullptr)
            return "";

        if (node->gatorId == gatorId)
            return node->name;

        string foundName = searchById(node->left, gatorId);
        if (!foundName.empty())
            return foundName;

        return searchById(node->right, gatorId);
    }

    void searchByName(Node* node, string name) {
        if (node == nullptr)
            return;

        if (node->name == name) {
            cout << node->gatorId << endl;
            found = true;
        }

        searchByName(node->left, name);
        searchByName(node->right, name);
    }

    Node* findMin(Node* node) {
        if (node == nullptr) {
            return nullptr;
        }

        while (node->left != nullptr) {
            node = node->left;
        }

        return node;
    }

    Node* removeNode(Node* root, int targetID) {
        if (root == nullptr) {
            cout << "unsuccessful" << endl;  // Target node not found
            return nullptr;
        } else if (targetID < root->gatorId) {
            root->left = removeNode(root->left, targetID);
        } else if (targetID > root->gatorId) {
            root->right = removeNode(root->right, targetID);
        } else {
            found = true;
            if (root->left == nullptr) {
                Node* successor = root->right;
                delete root;
                return successor;
            } else if (root->right == nullptr) {
                Node* successor = root->left;
                delete root;
                return successor;
            } else {
                Node* successor = findMin(root->right);
                root->gatorId = successor->gatorId;
                root->name = successor->name;
                root->right = removeNode(root->right, successor->gatorId);
            }
        }

        return root;
    }
    int countNodes(Node* node) {
        if (node == nullptr)
            return 0;

        int leftCount = countNodes(node->left);
        int rightCount = countNodes(node->right);

        return 1 + leftCount + rightCount;
    }

    Node* removeInorderPos(Node* root, int& count, int inorderIndex) {
        if (root == nullptr) {
            return nullptr;
        }

        root->left = removeInorderPos(root->left, count, inorderIndex);

        count++;
        if (count - 1 == inorderIndex) {
            root = removeNode(root, root->gatorId);
            return root;
        }

        root->right = removeInorderPos(root->right, count, inorderIndex);

        return root;
    }


// -------------------------------------------------------------------------------------------------///////
public:

    void removeInorder(int inorderIndex) {
        int count = 0;
        root = removeInorderPos(root, count, inorderIndex);
    }

    int countNodes() {
        return countNodes(root);
    }
    AVLTree() : root(nullptr) {}

    void insert(const string& name, int gatorId) {
        duplicate = false;
        root = insertNode(root, name, gatorId);

        duplicate ? cout << "unsuccessful" << endl: cout << "successful" << endl;
    }

    void remove(int gatorID){
        found = false;
        removeNode(root, gatorID);
        if (found){
            cout << "successful" << endl;
        }
    }


    void search(int gatorId) {
        string name = searchById(root, gatorId);

        if (!name.empty()) {
            cout << name << endl;
        } else {
            cout << "unsuccessful" << endl;
        }
    }


    void search(const string& name) {
        found = false; // Reset the flag before searching
        searchByName(root, name);
        if (!found)
            cout << "unsuccessful" << endl;
    }

    void printLevelCount() {
        int count = getLevelCount(root);
        cout << count << endl;
    }

    void printPreorder() {
        cout << Preorder(root) << endl;
    }

    void printInorder() {
        cout << Inorder(root) << endl;
    }

    void printPostorder() {
        cout << Postorder(root) << endl;
    }

};

bool ValidName(const string& name)
{
    // Check if the name matches the regex pattern
    regex pattern("^\"[A-Za-z\\s]*\"$");
    return regex_match(name, pattern);
}

bool ValidID(const string& id)
{
    // Check if the ID is a number consisting of 8 digits
    regex pattern("^\\d{8}$");
    return regex_match(id, pattern);
}

int main()
{
    AVLTree myAVL;

    int num;
    cin >> num;

    for (int i = 0; i < num; i++) {
        string command, input1, input2;

        cin >> command;

        if (command.compare("insert") == 0) {
            cin >> input1;


            // Read the remaining inputs until a closing double quote is found
            while (input1.at(input1.size() - 1) != '"') {
                cin >> input2;
                input1.append(" ");
                input1.append(input2);
            }


            cin >> input2;


            // Check if the second input is a valid ID
            if (!ValidID(input2)) {
                cout << "unsuccessful" << endl;
                continue;
            }

            // Check if the first input is a valid name
            if (!ValidName(input1)) {
                cout << "unsuccessful" << endl;
                continue;
            }

            int id = stoi(input2);
            myAVL.insert(input1.substr(1, input1.size() - 2), id);

        }
        else if (command.compare("search") == 0){
            cin.ignore(); // Ignore any leading whitespace
            getline(cin, input1); // Read the entire line as input1

            if (ValidName(input1)){
                myAVL.search(input1.substr(1, input1.size() - 2));
            }
            else if (ValidID(input1)){
                myAVL.search(stoi(input1));
            }
            else{
                cout << "unsuccessful" << endl;
                continue;
            }
        }

        else if (command.compare("printLevelCount") == 0){
            if (command.size() > 15) {
                cout << "unsuccessful" << endl;
                continue;
            }
            myAVL.printLevelCount();
        }
        else if (command.compare("printPreorder") == 0){
            if (command.size() > 13) {
                continue;
            }
            myAVL.printPreorder();
        }
        else if (command.compare("printInorder") == 0){
            if (command.size() > 12) {
                continue;
            }
            myAVL.printInorder();
        }
        else if (command.compare("printPostorder") == 0){
            if (command.size() > 14) {
                cout << "unsuccessful" << endl;
                continue;
            }
            myAVL.printPostorder();
        }
        else if (command.compare("remove") == 0){
            cin.ignore(); // Ignore any leading whitespace
            getline(cin, input1); // Read the entire line as input1

            if (ValidID(input1)){
                myAVL.remove(stoi(input1));
            }
            else{
                cout << "unsuccessful" << endl;
                continue;
            }
        }
        else if (command.compare("removeInorder") == 0){
            cin.ignore(); // Ignore any leading whitespace
            getline(cin, input1); // Read the entire line as input1

            if (stoi(input1) > myAVL.countNodes() - 1){
                cout << "unsuccessful" << endl;
            }
            else{
                myAVL.removeInorder(stoi(input1));
                cout << "successful" << endl;
            }
        }
    }

    return 0;
}
