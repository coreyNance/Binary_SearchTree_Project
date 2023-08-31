//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : Corey Nance
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
// Date        : 2/14/2021
// Course      : CS 260
// Project #   : 6-2 Programming Activity: Trees
//============================================================================

// libraries to be used in this program.
#include <iostream>
#include <algorithm>
#include <climits>
#include <time.h>
#include <string>
#include "CSVparser.hpp"

using namespace std;

// Global definitions visible to all methods and classes
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

//Internal structure for tree node
struct Node {

	Bid bid;
	Node* left;
	Node* right;

	//default constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}

	// initialize with a given bid
	Node(Bid aBid) : Node() {

		this->bid = aBid;
	}

};

// Binary Search Tree class definition
class BinarySearchTree {

	// Private class functions with the root node pointer.
private:
    Node* root;

    void addNode(Node* node, Bid bid);
    void inOrder(Node* node);
    Node* removeNode(Node* node, string bidId);


    // Public class functions to allow access to class functions.
public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Bid bid);
    void Remove(string bidId);
    Bid Search(string bidId);
};

//Default constructor
BinarySearchTree::BinarySearchTree() {
    // initialize housekeeping variables
	root = nullptr;
}

// Destructor
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
}

//Traverse the tree in order
void BinarySearchTree::InOrder() {
	// Function call to the inOrder Private function
	inOrder(root);
}
// Inserting a bid into the tree
void BinarySearchTree::Insert(Bid bid) {
   // If statement to check if the root is null and if not call addNode
	if (root == nullptr) {
		root = new Node(bid);
	}
	else {
		this->addNode(root, bid);
	}
}

// Removing a bid from the tree
void BinarySearchTree::Remove(string bidId) {

    // function call to remove Node.
	this->removeNode(root, bidId);
}

// Searching the tree for a bid
Bid BinarySearchTree::Search(string bidId) {


	// start searching from the root
	Node* current = root;

	// keep looping downwards until bottom reached or bid is found
	while(current != nullptr) {

		// if current node matches, return it
		if (current->bid.bidId.compare(bidId) == 0) {
			return current->bid;
		}
		// if bid is smaller than current than traverse left
		if (bidId.compare(current->bid.bidId) < 0) {
			current = current->left;
		}
		else {
			current = current->right;
		}
	}

	Bid bid;
    return bid;
}

//Inserting a bid into the tree
void BinarySearchTree::addNode(Node* node, Bid bid) {


	//if node is larger than the bid add to left subtree
	if (node->bid.bidId.compare(bid.bidId) > 0) {
		if(node->left == nullptr) {
			node->left = new Node(bid);
		}
		else {
			this->addNode(node->left, bid);
		}

	}
	// add to right subtree
	else {
		if(node->right == nullptr) {
					node->right = new Node(bid);
				}
				else {
					this->addNode(node->right, bid);
				}
	}
}
//Function to print out all nodes from the tree
void BinarySearchTree::inOrder(Node* node) {

	// If statemement to check if the root if not empty
	if(root != NULL) {
		// nested if statement check if left node point is not pointed to null call inOrder again
		if(node->left != NULL) {
			inOrder(node->left);
		}
		//Output statement if left node is pointed to null print output.
		cout << node->bid.bidId << ": " << node->bid.title << " | " << node->bid.amount << " | "
		            << node->bid.fund << endl;

		// after output is printed, this if statement checks if right pointer is pointing to null.
		if(node->right != NULL){
			inOrder(node->right);
		}
	}
	else {
		cout << "The tree is empty" << endl;
	}
}

//Function to remove a node from the Tree.
Node* BinarySearchTree::removeNode(Node* node, string bidId) {

	// if this node is null then return (avoid crashing)
	if (node == nullptr) {
		return node;
	}

	// go down left subtree
	if (bidId.compare(node->bid.bidId) < 0) {
		node->left = removeNode(node->left, bidId);
	}
	else if (bidId.compare(node->bid.bidId) > 0){
		node->right = removeNode(node->right, bidId);
		}
	else {
		// no children on
		if (node->left == nullptr && node->right == nullptr) {
			delete node;
			node = nullptr;
		}
		// one child to the left
		else if (node->left != nullptr && node->right == nullptr) {
			Node* temp = node;
			node = node-> left;
			delete temp;
		}
		else if (node->right != nullptr && node->left == nullptr) {
				Node* temp = node;
				node = node-> right;
				delete temp;
		}
		else {
			Node*temp = node->right;
			while (temp->left != nullptr) {
				temp = temp->left;
			}
			node->bid = temp->bid;
			node->right = removeNode(node->right, temp->bid.bidId);
		}
	}
	return node;
}


// Static methods used for testing
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
            << bid.fund << endl;
    return;
}

// Load a CSV file containing bids into a container
void loadBids(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            bst->Insert(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

// The one and only main() method
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98109";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
        bidKey = "98109";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a binary search tree to hold all bids
    BinarySearchTree* bst;

    Bid bid;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Bid" << endl;
        cout << "  4. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            bst = new BinarySearchTree();

            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            loadBids(csvPath, bst);

            //cout << bst->Size() << " bids read" << endl;

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            bst->InOrder();
            break;

        case 3:
            ticks = clock();

            bid = bst->Search(bidKey);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
            	cout << "Bid Id " << bidKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 4:
            bst->Remove(bidKey);
            break;
        }
    }

    cout << "Good bye." << endl;

	return 0;
}
