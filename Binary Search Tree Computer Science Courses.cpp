// Project intakes a csv of course information and loads it into a binary search tree.
#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <string>

using namespace std;

struct Course {
    string number;
    string name;
    vector<string> prereqs;
    Course() = default;
    Course(const string& number, const string& name, const vector<string>& prereqs = {}) : number(number), name(name), prereqs(prereqs) {};
};

struct Node {
    Course course;
    Node* left;
    Node* right;
    Node() {
        left = nullptr;
        right = nullptr;
    }
    Node(Course aCourse) : Node() { course = aCourse; };

};

class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);


public:
    BinarySearchTree();
    virtual ~BinarySearchTree();

    void deleteNodes(Node* node);
    void InOrder();

    void Insert(Course course);
    Course Search(string courseNumber);
    vector<Course> parseCourses(string path, char sep);

};

BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

/**
 * Recursive helper function to delete all nodes
*/
void BinarySearchTree::deleteNodes(Node* node) {
    if (node != nullptr) {
        deleteNodes(node->left);
        deleteNodes(node->right);
        delete node;
    }
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    //FixMe (2)
    // recurse from root deleting every node
    deleteNodes(root);
}


/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    // FixMe (3a): In order root
    // call inOrder fuction and pass root 
    inOrder(root);
}



/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {
    Node* newNode = new Node(course);
    newNode->left = nullptr;
    newNode->right = nullptr;
    if (root == nullptr) {
        root = newNode;
    }
    else {
        addNode(root, course);
    };

}



/**
 * Search for a course
 */
Course BinarySearchTree::Search(string courseNumber) { // Find a node with a desired key 
    Node* currNode = root;
    // keep looping downwards until bottom reached or matching courseNumber found
    // if match found, return current course
    while (currNode != nullptr) {
        if (currNode->course.number == courseNumber) {
            return currNode->course;
        }
        // if course is smaller than current node then traverse left
        else if (courseNumber < currNode->course.number) {
            currNode = currNode->left;
        }  // else larger so traverse right
        else if (courseNumber > currNode->course.number) {
            currNode = currNode->right;
        }
    }
    Course course;

    return course;
}

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param course Course to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {

    if (node->course.number > course.number) {
        if (node->left == nullptr) {
            node->left = new Node(course); // insert new node with course at left child
        }
        else {
            addNode(node->left, course); // recurse and keep going down the left tree
        }
    }
    else {
        if (node->right == nullptr) {
            node->right = new Node(course); // insert the new node with course at the right child
        }
        else {
            addNode(node->right, course); // recurse and keep doing down the right tree
        };
    };

}

void BinarySearchTree::inOrder(Node* node) {
    if (node != nullptr) {
        inOrder(node->left);
        cout << "Course ID: " << node->course.number << " | Name: " << node->course.name << " | Prerequisites: ";
        if (!node->course.prereqs.empty()) {
            for (size_t i = 0; i < node->course.prereqs.size(); ++i) {
                cout << node->course.prereqs[i];
                if (i != node->course.prereqs.size() - 1) {
                    cout << ", ";
                }
            }
        }
        else {
            cout << "None";
        }
        cout << endl;
        inOrder(node->right);
    };

}






vector<Course> parseCourses(string path, char sep) {
    vector<Course> courses;
    ifstream file(path);
    if (!file.is_open()) {
        throw runtime_error("File failed to open.");

    }

    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            vector<string> tokens;
            string currToken = "";
            for (char c : line) {
                if (c == sep) {
                    if (!currToken.empty()) {
                        tokens.push_back(currToken);
                        currToken = "";
                    }
                }
                else {
                    currToken += c;
                }
            }
            if (!currToken.empty()) {
                tokens.push_back(currToken);

            }

            if (tokens.size() < 2) {
                throw runtime_error("File format error, course details not all present");

            }

            vector<string> prereqs;

            if (tokens.size() > 2) {
                for (size_t i = 2; i < tokens.size(); ++i) {
                    prereqs.push_back(tokens[i]);
                }
            }

            Course course(tokens[0], tokens[1], prereqs);
            courses.push_back(course);
        }
    }
    return courses;
}

int main()
{
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course course;
    string file = "CS 300 ABCU_Advising_Program_Input.csv";
    string searchNumber;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Print Courses" << endl;
        cout << "  3. Search and Print Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {

        case 1:
            try {
                vector<Course> courses = parseCourses(file, ',');
                for (const Course& course : courses) {
                    bst->Insert(course);
                }
                cout << "Courses successfully parsed and inserted into BST." << endl;
            }
            catch (exception& ex) { cout << "Error reading file and parsing courses. " << ex.what(); }
            break;

        case 2:
            bst->InOrder();
            break;

        case 3: {
            cout << "Enter course number: " << endl;
            getline(cin, searchNumber);
            for (char& c : searchNumber) {
                c = toupper(c);
            }
            try {
                Course foundCourse = bst->Search(searchNumber);

                if (!foundCourse.number.empty()) {
                    cout << "Course ID: " << foundCourse.number << " | Name: " << foundCourse.name << " | Prerequisites: ";
                    for (size_t i = 0; i < foundCourse.prereqs.size(); ++i) {
                        cout << foundCourse.prereqs[i];
                        if (i != foundCourse.prereqs.size() - 1) {
                            cout << ", ";
                        }
                    }
                }
                else {
                    cout << "Course not found.";
                }
                cout << endl;
            }
            catch (const exception& ex) {
                cout << "Error finding course: " << ex.what() << endl;
            }
            break;
        }
        case 9:
            delete bst;
            return 0;
            
        }
    }
    return 0;
}
