#include "main.h"
int maxsize;
vector<vector<int>> c(201, vector<int>(201));

template <typename E> class HuffTree;
// Huffman tree node abstract base class
template <typename E> class HuffNode {
private:
	HuffNode<E>* lc;
	HuffNode<E>* rc;
	E it;
	int wgt;
	friend class HuffTree<E>;
public:
	HuffNode(const E& val, int freq) // Constructor
	{
		it = val; wgt = freq; lc = NULL, rc = NULL;
	}
	HuffNode(HuffNode<E>* l, HuffNode<E>* r)
	{
		wgt = l->weight() + r->weight(); lc = l; rc = r; it = 0;
	}
	int weight() { return wgt; }
	E val() { return it; }
	bool isLeaf() { return (!lc && !rc); }
	HuffNode<E>* left() const { return lc; }
	HuffNode<E>* right() const { return rc; }
	void setLeft(HuffNode<E>* b)
	{
		lc = (HuffNode<E>*)b;
	}
	void setRight(HuffNode<E>* b)
	{
		rc = (HuffNode<E>*)b;
	}
};

// HuffTree is a template of two parameters: the element
// type being coded and a comparator for two such elements.
template <typename E>
class HuffTree {
private:
HuffNode<E>* Root; // Tree root
int order;
friend class Restaurant;
public:
HuffTree(E val, int freq) // Leaf constructor
{ Root = new HuffNode<E>(val, freq); }
// Internal node constructor
HuffTree(HuffTree<E>* l, HuffTree<E>* r, int order)
{
	Root = new HuffNode<E>(l->root(), r->root()); this->order = order;
}
~HuffTree() {} // Destructor
HuffNode<E>* root() { return Root; } // Get root
int weight() { return Root->weight(); } // Root weight
int Order() { return order; }
void deleteTree(HuffNode<E>* root) {
	if (!root) return;
	deleteTree(root->lc);
	deleteTree(root->rc);
	delete root;
}

HuffNode<E>* rotateRight(HuffNode<E>* node) {
	//TODO: Rotate and return new root after rotate
	HuffNode<E>* temp = node->lc;
	node->lc = temp->rc;
	temp->rc = node;
	return temp;
}
HuffNode<E>* rotateLeft(HuffNode<E>* node) {
	//TODO: Rotate and return new root after rotate
	HuffNode<E>* temp = node->rc;
	node->rc = temp->lc;
	temp->lc = node;
	return temp;
}
int getHeightRec(HuffNode<E>* node)
{
	if (node == NULL)
		return 0;
	int lh = this->getHeightRec(node->lc);
	int rh = this->getHeightRec(node->rc);
	return (lh > rh ? lh : rh) + 1;
}
int getBalance(HuffNode<E>* node) {
	if (!node) return 0;
	return getHeightRec(node->lc) - getHeightRec(node->rc);
}
HuffNode<E>* firstUnbalanced(HuffNode<E>* node) {
	if (!node) return NULL;
	if (getBalance(node) >= 2 || getBalance(node) <= -2) return node;
	else  {
		if (firstUnbalanced(node->lc)) return firstUnbalanced(node->lc); 
		else return firstUnbalanced(node->rc);
	}
}
HuffNode<E>* parent(HuffNode<E>* node, HuffNode<E>* root, HuffNode<E>* par) {
	if (root == node) return par;
	if (!root) return NULL;
	if (parent(node, root->lc, root)) return parent(node, root->lc, root);
	else return parent(node, root->rc, root);
}

HuffNode<E>* balanceNode(HuffNode<E>*& node) {
	if (!node) return NULL;
	int balance = getBalance(node);
	if (balance > 1) {
		if (getHeightRec(node->lc->lc) >= getHeightRec(node->lc->rc)) return rotateRight(node);
		else {
			node->lc = rotateLeft(node->lc);
			return rotateRight(node);
		}
	}
	else if (balance < -1) {
		if (getHeightRec(node->rc->lc) <= getHeightRec(node->rc->rc)) return rotateLeft(node);
		else {
			node->rc = rotateRight(node->rc);
			return rotateLeft(node);
		}
	}
	return node;
}
void balanceTree(int& count) {
	while (firstUnbalanced(Root) && count < 3) {
		HuffNode<E>* x = firstUnbalanced(this->Root);
		HuffNode<E>* y = parent(x,Root,NULL);
		if (!y) Root = balanceNode(x);
		else if (y->rc == x) y->rc = balanceNode(x);
		else y->lc = balanceNode(x);
		count++;
	}
}
void balanceTree(HuffNode<E>* node, int& count) {
	if (!node || count == 3) return;
	while ((getBalance(node) > 1 || getBalance(node) < -1) && count < 3) {
		HuffNode<E>* x = balanceNode(node);
		HuffNode<E>* y = parent(node, Root, NULL);
		if (!y) Root = x;
		else if (y->rc == node) y->rc = x;
		else y->lc = x;
		node = x;
		count++;
	}
	balanceTree(node->lc, count);
	balanceTree(node->rc, count);
}
};

class compareHuffTree
{
public:
	bool operator() (HuffTree<char>* lhs, HuffTree<char>* rhs) const
	{
		HuffNode<char>* a = lhs->root(), * b = rhs->root();
		if (a->weight() != b->weight()) return (a->weight() < b->weight());
		else if (a->isLeaf() && b->isLeaf()) {
			if ((a->val() >= 'a' && a->val() <= 'z') && (b->val() >= 'A' && b->val() <= 'Z')) return 1;
			else if ((a->val() >= 'A' && a->val() <= 'Z') && (b->val() >= 'a' && b->val() <= 'z')) return 0;
			else return a->val() < b->val();
		}
		else if (a->isLeaf() && !b->isLeaf()) return 1;
		else if (!a->isLeaf() && b->isLeaf()) return 0;
		else return lhs->Order() < rhs->Order();
	}
};

void preOrder(HuffNode<char>* treeNode) {
	if (!treeNode)  return;
	cout << treeNode->val() << " " << treeNode->weight() << endl;
	preOrder(treeNode->left());
	preOrder(treeNode->right());
}
void inOrder(HuffNode<char>* treeNode) {
	if (!treeNode)  return;
	inOrder(treeNode->left());
	if (treeNode->isLeaf()) cout << treeNode->val() << '\n';
	else cout << treeNode->weight() << '\n';
	inOrder(treeNode->right());
}
void traverseCode(HuffNode<char>* node, string s, map<char,string>& code) {
	if (node->left()) traverseCode(node->left(), s + '0', code);
	if (node->right()) traverseCode(node->right(), s + '1', code);
	if (node->val()) code[node->val()] = s;
}
int BinToDec(string s) {
	int res = 0;
	for (int i = s.size() - 1, exp = 1; i >= 0; i--, exp *= 2) {
		res += (s[i] - '0') * exp;
	}
	return res;
}

//BSTNode
class Node
{
private:
	int value;
	int time;
	Node* pLeft;
	Node* pRight;
	friend class BinarySearchTree;
	friend class Restaurant;
public:
	Node(int value, int time) : value(value), time(time), pLeft(NULL), pRight(NULL) {}
	~Node() {}
};
//Binary Search Tree
class BinarySearchTree
{
private:
	Node* root;
	int order;
public:
	BinarySearchTree() : root(nullptr), order(0) {}
	~BinarySearchTree() {}

	Node* Root() { return root; }
	int Order() { return order; }
	void add(int value) {
		Node* node = new Node(value, ++order);
		if (root == NULL) { root = node; return; }
		Node* temp = root, * parent = root;
		while (temp) {
			parent = temp;
			if (temp->value > value) temp = temp->pLeft;
			else temp = temp->pRight;
		}
		if (parent->value > value) parent->pLeft = node;
		else parent->pRight = node;
	}

	void deleteNode(Node* node) {
		Node* iter = root, * par = NULL;
		while (iter && iter->time != node->time) {
			par = iter;
			if (iter->value > node->value) iter = iter->pLeft;
			else iter = iter->pRight;
		}
		if (!iter) return;
		if (!iter->pLeft && !iter->pRight) {
			if (iter == root) root = NULL;
			else if (par->pLeft == iter) par->pLeft = NULL;
			else par->pRight = NULL;
		}
		else if (!iter->pLeft || !iter->pRight) {
			Node* child = iter->pLeft ? iter->pLeft : iter->pRight;
			if (!par) root = child;
			else if (par->pLeft == iter) par->pLeft = child;
			else par->pRight = child;
		}
		else {
			Node* cur = iter;
			par = iter, iter = iter->pRight;
			while (iter->pLeft) par = iter, iter = iter->pLeft;
			cur->value = iter->value, cur->time = iter->time;
			if (par->pLeft == iter) par->pLeft = iter->pRight;
			else par->pRight = iter->pRight;
		}
		//delete iter;
	}
	void deleteTree(Node* root) {
		if (!root) return;
		deleteTree(root->pLeft);
		deleteTree(root->pRight);
		delete root;
	}
	void inOrderPrint(Node* root) {
		if (!root) return;
		inOrderPrint(root->pLeft);
		cout << root->value << '\n';
		inOrderPrint(root->pRight);
	}
	int numCount(Node* root) {
		if (!root) return 0;
		return numCount(root->pLeft) + numCount(root->pRight) + 1;
	}
	int permutation(Node* root) {
		if (!root) return 1;
		int x = numCount(root->pLeft), y = numCount(root->pRight);
		return (((permutation(root->pLeft) * permutation(root->pRight)) % maxsize) * c[x][x + y]) % maxsize;
	}
};

class nodeHeap {
private:
	int ID;
	int lastChange;
	int num;
	vector<int> result;
	friend class Restaurant;
	friend class minHeap;
public:
	nodeHeap(int ID, int lastChange, int result) {
		this->ID = ID, this->lastChange = lastChange, this->num = 1;
		this->result.push_back(result);
	}
	int getNum() { return num; }
	int getLastChange() { return lastChange; }
	void decreaseAndPrint(int people) {
		num = max(num - people, 0);
		while (people-- && !result.empty()) {
			cout << result[0] << '-' << ID << '\n';
			result.erase(result.begin());
		}
	}
};
bool compareHeapNode (nodeHeap a, nodeHeap b) {
	if (a.getNum() != b.getNum()) return a.getNum() < b.getNum();
	else return a.getLastChange() < b.getLastChange();
}


class minHeap {
protected:
	vector<nodeHeap> elements;
	int count;
	int order;
	friend class Restaurant;
public:
	minHeap()
	{
		this->count = 0; order = 0;
	}
	~minHeap(){}

	void push(nodeHeap a) {
		count++;
		elements.push_back(a);
		reheapUp(count - 1);
	}
	void printHeap()
	{
		cout << "Min Heap [ ";
		for (int i = 0; i < count; i++)
			cout << elements[i].ID << " " << elements[i].num << " " << elements[i].lastChange << " - ";
		cout << "]\n";
	}

	bool isLeaf(int pos, int n) {
		return (pos >= n / 2 && pos < n);
	}
	void reheapUp(int position) {
		int curr = position;
		while (curr != 0 && compareHeapNode(elements[curr], elements[(curr - 1) / 2])) {
			swap(elements[curr], elements[(curr - 1) / 2]);
			curr = (curr - 1) / 2;
		}
	}
	void reheapDown(int position) {
		int curr = position;
		while (!isLeaf(curr, count)) {
			int j = 2 * curr + 1, rc = 2 * curr + 2;
			if (rc < count && !compareHeapNode(elements[j], elements[rc])) j = rc;
			if (compareHeapNode(elements[curr], elements[j])) return;
			swap(elements[curr], elements[j]);
			curr = j;
		}
	}

	bool contains(int ID) {
		for (int i = 0; i < count; i++) {
			if (elements[i].ID == ID) return true;
		}
		return false;
	}
	int getID(int ID) {
		// TODO: return the index of ID in heap
		for (int i = 0; i < count; i++) {
			if (elements[i].ID == ID) return i;
		}
		return -1;
	}
	void remove(int ID) {
		// TODO: remove the element with value equal to ID
		int pos = getID(ID);
		if (pos == count - 1) { count--; elements.pop_back(); }
		else {
			swap(elements[pos], elements[count - 1]);
			elements.pop_back();
			count--;
			reheapUp(pos);
			if (count != 0) reheapDown(pos);
		}
	}
	void clear() {
		elements.clear();
		count = 0, order = 0;
	}

	void PrintInOrder(int num, int pos) {
		if (pos >= count) return;
		int num1 = min(elements[pos].num, num);
		for (int i = elements[pos].num - 1; i >= elements[pos].num - num1; i--) {
			cout << elements[pos].ID << '-' << elements[pos].result[i] << '\n';
		}
		PrintInOrder(num, 2 * pos + 1);
		PrintInOrder(num, 2 * pos + 2);
	}
};


class Restaurant {
private:
	unordered_map<int, BinarySearchTree> Gojo;
	unordered_map<int, queue<Node*>> queueG;
	minHeap Sakuna;
	HuffTree<char>* latest;
	HuffTree<char>* copy;
public:
	Restaurant() { latest = NULL, copy = NULL; }
	~Restaurant() { 
		if(latest) latest->deleteTree(latest->Root);
		//if(copy) copy->deleteTree(copy->Root);
		for (int i = 1; i <= maxsize; i++) Gojo[i].deleteTree(Gojo[i].Root());
		for (int i = 1; i <= maxsize; i++) {
			while (!queueG[i].empty()) {
				Node* a = queueG[i].front();
				queueG[i].pop(); delete a;
			}
		}
	}
	void LAPSE(string name) {
		//Convert customer's name
		map<char, int> s;
		for (unsigned int i = 0; i < name.size(); i++) { s[name[i]]++; }
		if (s.size() <= 2) return;
		for (unsigned int i = 0; i < name.size(); i++) {
			if (name[i] >= 'a' && name[i] <= 'z') {
				if (s[name[i]] % 26 > 'z' - name[i]) name[i] -= 26 - s[name[i]] % 26;
				else name[i] += s[name[i]] % 26;
			}
			else {
				if (s[name[i]] % 26 > 'Z' - name[i]) name[i] -= 26 - s[name[i]] % 26;
				else name[i] += s[name[i]] % 26;
			}
		}
		//cout << name << endl;
		set<HuffTree<char>*, compareHuffTree> q;
		s.clear();
		for (unsigned int i = 0; i < name.size(); i++) { s[name[i]]++; }
		for (auto i : s) {
			HuffTree<char>* node = new HuffTree<char>(i.first, i.second);
			q.insert(node);
		}
		//for (auto i : q) cout << i->root()->val() << i->root()->weight() << " "; cout << endl;
		HuffTree<char>* lTree = NULL, * rTree = NULL, * Tree = NULL;
		int order = 1;
		if (q.size() == 1) Tree = *q.begin();
		else {
			while (q.size() > 1) {
				lTree = *q.begin(); q.erase(q.begin());
				rTree = *q.begin(); q.erase(q.begin());
				Tree = new HuffTree<char>(lTree, rTree, order);
				order++; int t = 0;
				Tree->balanceTree(t);
				//preOrder(Tree->root()); cout << endl;
				q.insert(Tree);
				delete lTree; delete rTree;
			}
		}
		//preOrder(Tree->root());
		if(copy) copy->deleteTree(copy->Root);
		latest = Tree, copy = Tree;
		map<char, string> code;
		int result = 0;
		if (Tree->root()->isLeaf() == 0) {
			traverseCode(Tree->root(), "", code);
			string encode = "";
			for (unsigned int i = 0; i < name.size(); i++) { encode += code[name[i]]; }
			if (encode.size() > 10) encode = encode.substr(encode.size() - 10);
			reverse(encode.begin(), encode.end());
			//cout << encode << endl;
			result = BinToDec(encode);
		}
		//cout << result << endl;

		//Choose restaurant
		if (result % 2 == 1) {// Gojo-BST
			int ID = result % maxsize + 1;
			Gojo[ID].add(result);
			Node* a = new Node(result, Gojo[ID].Order());
			queueG[ID].push(a);
			if (ID == maxsize) { cout << result << " Hereeeeeeeeeeeeeeeeeeeeeeeee" << endl; LIMITLESS(ID); }
		}
		else {// Sakuna-minHeap
			int ID = result % maxsize + 1;
			if (Sakuna.contains(ID)) {
				int pos = Sakuna.getID(ID);
				Sakuna.elements[pos].num++;
				Sakuna.order++;
				Sakuna.elements[pos].lastChange = Sakuna.order;
				Sakuna.elements[pos].result.push_back(result);
				Sakuna.reheapDown(pos);
			}
			else {
				nodeHeap a(ID, ++Sakuna.order, result);
				Sakuna.push(a);
			}
			//Sakuna.printHeap();
		}
	}
	void KOKUSEN() {
		cout << Gojo[maxsize].permutation(Gojo[maxsize].Root()) << endl;
		for (int i = 1; i <= maxsize; i++) {
			if (Gojo[i].Root()) {
				unsigned int num = Gojo[i].permutation(Gojo[i].Root());
				//cout << num << " " << queueG[i].size() << endl;
				num = queueG[i].size() <= num ? queueG[i].size() : num;
				for (unsigned int j = 1; j <= num; j++) {
					Gojo[i].deleteNode(queueG[i].front());
					queueG[i].pop();
				}
				//LIMITLESS(i);
			}
		}
	}
	void KEITEIKEN(int num) {
		int num1 = min(num, Sakuna.count);
		vector<nodeHeap> copy(Sakuna.elements); 
		sort(copy.begin(), copy.end(), compareHeapNode);
		/*for (int i = 0; i < copy.size(); i++) {
			cout << copy[i].ID << " " << copy[i].num << " " << copy[i].lastChange << endl;
		}*/
		for (int i = 0; i < num1; i++) {
			int decreaseID = copy[i].ID;
			int pos = Sakuna.getID(decreaseID);
			Sakuna.elements[pos].decreaseAndPrint(num);
			Sakuna.elements[pos].lastChange = ++Sakuna.order;
			if (Sakuna.elements[pos].num == 0) Sakuna.remove(decreaseID);
			else Sakuna.reheapUp(pos);
			//Sakuna.printHeap();
		}
	}
	void HAND() {
		if (!latest) return;
		inOrder(latest->root());
	}
	void LIMITLESS(int num) {
		if (num <= 0 || num > maxsize) return;
		if (Gojo.find(num) == Gojo.end()) return;
		Gojo[num].inOrderPrint(Gojo[num].Root()); 
	}
	void CLEAVE(int num) {
		Sakuna.PrintInOrder(num, 0); 
	}
};

void simulate(string filename) {
	ifstream ss(filename);
	string str, name;
	int num, line = 1;
	Restaurant* r = new Restaurant();
	while (ss >> str) {
		cout << "line " << line << ": ";
		line++;
		if (str == "MAXSIZE") {
			string Maxsize; ss >> Maxsize;
			maxsize = stoi(Maxsize);
			cout << "MAXSIZE" << " " << maxsize << endl;
			for (int i = 0; i <= 200; i++) c[0][i] = 1;
			for (int i = 0; i <= 200; i++) c[i][i] = 1;
			for (int i = 1; i <= 200; i++) {
				for (int j = i + 1; j <= 200; j++) {
					c[i][j] = (c[i][j - 1] + c[i - 1][j - 1]) % maxsize;
				}
			}
		}
		else if (str == "LAPSE") {
			ss >> name;
			cout << "LAPSE " << name << endl;
			r->LAPSE(name);
		}
		else if (str == "KOKUSEN") {
			cout << "KOKUSEN" << endl;
			r->KOKUSEN();
		}
		else if (str == "KEITEIKEN") {
			string Num; ss >> Num;
			num = stoi(Num);
			cout << "KEITEIKEN " << num << endl;
			r->KEITEIKEN(num);
		}
		else if (str == "HAND") {
			cout << "HAND" << endl;
			r->HAND();
		}
		else if (str == "LIMITLESS") {
			string Num; ss >> Num;
			num = stoi(Num);
			cout << "LIMITLESS " << num << endl;
			r->LIMITLESS(num);
		}
		else { // CLEAVE <NUM>
			string Num; ss >> Num;
			num = stoi(Num);
			cout << "CLEAVE " << num << endl;
			r->CLEAVE(num);
		}
	}
	delete r;
}
