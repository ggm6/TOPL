#include <iostream>
#include <string>
#include <stack>
#include <vector>

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
bool evalAST(node*& p);



int main() {

	//string exp{"!true||false&&false"};
	string exp{"!((!false)||true)"};
	//string exp{"true"};
	node* treeHead = nullptr;
	createAST(treeHead,exp);
	bool ans = evalAST(treeHead);
	cout << ans << endl;
	

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
//	for (i;i<end;++i) {  // !((!false)||true)
	//cout << "i: " << *i << endl;
		if (*i=="!") {
//			cout << "exc" << endl;
			auto dummy = i+1;
			resolveExp(end,p,dummy);
//			cout << "After" << endl;
			node* temp = p;
			p = new node{*i,temp};
			cout << p->Expression << endl;
			i = dummy+1;
			resolveExp(end,p,i);
		}
		else if (*i=="true" || *i=="false") {
			node* temp = p;
			p = new node{*i,temp};
			cout << p->Expression << endl;
			//cout << *(i-1) << endl;
			if (*(i-1)=="!")
				return;

			resolveExp(end,p,++i);
		}
		else if (*i=="||" || *i=="&&") {
			node* temp = p;
			p = new binaryOp;
			p->Expression = *i;
			cout << p->Expression << endl;
			static_cast<binaryOp*>(p)->leftChild = temp;
			temp = nullptr;
			resolveExp(end,temp,++i);
			static_cast<binaryOp*>(p)->rightChild = temp;
		}
		else if (*i=="(")
			resolveExp(end,p,++i);
//	}
}

bool evalAST(node*& p) {
	if (p->succ != nullptr)
		return evalAST(p->succ);
	else if (p->Expression=="!")
		return !evalAST(p->succ);
	else if (p->Expression=="||")
		return evalAST(static_cast<binaryOp*>(p)->leftChild) || evalAST(static_cast<binaryOp*>(p)->rightChild);
	else if (p->Expression=="&&")
		return evalAST(static_cast<binaryOp*>(p)->leftChild) && evalAST(static_cast<binaryOp*>(p)->rightChild);
	else if (p->Expression=="true")
		return true;

	//cout << p->Expression << endl;

	return false;

}