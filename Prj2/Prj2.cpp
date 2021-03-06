#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <assert.h>
#include <typeinfo>

using namespace std;

class node {
public:
	string Expression;
	node * succ;

	node() : succ(nullptr),Expression("") {};
	node(string s) : succ(nullptr),Expression(s) {};
	node(string s,node* p) : Expression(s),succ(p) {};
};

class binaryOp : public node {
public:
	node * leftChild;
	node * rightChild;

	binaryOp() : leftChild(nullptr),rightChild(nullptr) {};

};


void createAST(node*& head,const string& str);  // Essentially a wrapper
void Vectorize(vector<string>& v, const string& str);  // Turns string expression into vector of strings
void resolveExp(const vector<string>::iterator& end,node*& p,vector<string>::iterator& i); // Resolves expression of vector<string> into AST
void reduce(node*& p);
bool step(node*& p);
void stepSize(node*&p,int& size);
void stepHeight(node*& p,int& height);
bool areSame(const node* t1,const node* t2);
int size(node*& p);
int height(node*& p);



int main() {

	//string exp{"!true||false&&false"};    // Result = false
	//string exp{"!((!false)||true)"};    // Result = false
	string exp{"true||false"};                 // Result = true
	node* t1 = nullptr;
	createAST(t1,exp);
	//int s= height(t1);
	//cout << s << endl;
	//reduce(t1);	  // Final value of AST
	//string exp2{"!true||false&&false"};
	//string exp2{"!((!false)||true)"};
	//string exp2{"true"};
	//node* t2 = nullptr;
	//createAST(t2,exp2);
	//cout << areSame(t1,t2) << endl;
	

	return 0;
}



void Vectorize(vector<string>& v, const string& stmt) {
	for (auto i=0; i<stmt.length(); ++i) {
		if (stmt.substr(i,4)=="true") {
			v.push_back("true");
			i+=3;
		}
		else if (stmt.substr(i,5)=="false") {
			v.push_back("false");
			i+=4;
		}
		else if (stmt[i]=='!' || stmt[i]=='(' || stmt[i]==')') {
			string s;
			s.push_back(stmt[i]);
			v.push_back(s);
		}
		else if (stmt.substr(i,2)=="||" || stmt.substr(i,2)=="&&") {
			string s = stmt.substr(i,2);
			v.push_back(s);
			++i;
		}
	}
}

void createAST(node*& head,const string& str) {
	vector<string> v;
	Vectorize(v,str);
	vector<string>::iterator i = v.begin();
	vector<string>::iterator end = v.end();
	resolveExp(end,head,i);
}

void resolveExp(const vector<string>::iterator& end,node*& p,vector<string>::iterator& i) {
	if (*i=="!") {   
		auto dummy = i+1;   // It was either throw "i+1" into the function using a cast, or use a dummy variable
		if (dummy!=end)
			resolveExp(end,p,dummy);
		node* temp = p;
		p = new node{*i,temp};
		i = dummy+1;
		if (i!=end)
			resolveExp(end,p,i);
	}
	else if (*i=="true" || *i=="false") {
		node* temp = p;
		p = new node{*i,temp};
		if (i+1!=end)
			resolveExp(end,p,++i);
	}
	else if (*i=="||" || *i=="&&") {
		node* temp = p;
		p = new binaryOp;
		p->Expression = *i;
		static_cast<binaryOp*>(p)->leftChild = temp;
		temp = nullptr;
		if (i+1!=end)
			resolveExp(end,temp,++i);
		static_cast<binaryOp*>(p)->rightChild = temp;
	}
	else if (*i=="(" && i+1!=end)
		resolveExp(end,p,++i);
}

bool step(node*& p) {
	if (p->Expression=="!")
		return !step(p->succ);
	else if (p->Expression=="||")
		return step(static_cast<binaryOp*>(p)->leftChild) || step(static_cast<binaryOp*>(p)->rightChild);
	else if (p->Expression=="&&")
		return step(static_cast<binaryOp*>(p)->leftChild) && step(static_cast<binaryOp*>(p)->rightChild);
	else if (p->Expression=="true")
		return true;
	else if (p->Expression=="false")
		return false;
}

void stepSize(node*& p,int& size) {
	if (p->Expression=="!")
		++size,stepSize(p->succ,size);
	else if (p->Expression=="||" || p->Expression=="&&") {  // Prevent optimization
		++size;
		stepSize(static_cast<binaryOp*>(p)->leftChild,size);
		stepSize(static_cast<binaryOp*>(p)->rightChild,size);
	}
	else if (p->Expression=="true" || p->Expression=="false")
		++size;
}

void stepHeight(node*& p,int& height) {  // height by edges instead of nodes, i.e: height of "true" = 0
	if (p->Expression=="!")
		stepHeight(p->succ,++height);
	else if (p->Expression=="||" || p->Expression=="&&") {  // Prevent optimization
		int height1 = 0,height2 = 0;
		stepHeight(static_cast<binaryOp*>(p)->leftChild,++height1);
		stepHeight(static_cast<binaryOp*>(p)->rightChild,++height2);
		if (height1 > height2)
			height += height1;
		else
			height += height2;
	}
}

void reduce(node*& p) {   // Evaluation wrapper to house recursive function
	int size = 0,height = 0;
	bool ans = step(p);
	cout << "Ans: ";
	if (ans==true)
		cout << "true" << endl;
	else
		cout << "false" << endl;
}

bool areSame(const node* t1,const node* t2) {
	if (t1->Expression == t2->Expression) {
		if (t1->succ!=nullptr && t2->succ!=nullptr)
			return areSame(t1->succ,t2->succ);
		else if (t1->Expression=="||" || t1->Expression=="&&")
			return areSame(static_cast<const binaryOp*>(t1)->leftChild,static_cast<const binaryOp*>(t2)->leftChild) && areSame(static_cast<const binaryOp*>(t1)->rightChild,static_cast<const binaryOp*>(t2)->rightChild);
		else if ((t1->succ==nullptr && t2->succ!=nullptr) || (t2->succ==nullptr && t1->succ!=nullptr))
			return false;

		return true;
	}

	return false;
}

int size(node*& p) {
	int size = 0;
	stepSize(p,size);
	return size;
}

int height(node*& p) {
	int height = 0;
	stepHeight(p,height);
	return height;
}