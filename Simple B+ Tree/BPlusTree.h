/* ASSUMPTIONS:
1. When a node must be split and has an odd number of keys, the extra key will go to the new right-hand node.
2. There will never be a duplicate key passed to insert.
*/

#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


#ifndef DS_BPLUSTREE
#define DS_BPLUSTREE

///////////////////////////////////////////////////////////////////////////////
//DO NOT CHANGE THIS CODE
///////////////////////////////////////////////////////////////////////////////
template <class T> class BPlusTree;

template <class T>
class BPlusTreeNode{
public:
	BPlusTreeNode() : parent(NULL) {};
	bool is_leaf();
	bool contains(const T& key);

	//For grading only, this is actually bad to have because
	//it exposes a private member variable.
	BPlusTreeNode* get_parent() { return parent; } 

	//We need this to let BPlusTree access private members
	friend class BPlusTree<T>; 
private:
	bool contains(const T& key,std::size_t low,std::size_t high);
	std::vector<T> keys;
	std::vector<BPlusTreeNode*> children;
	BPlusTreeNode* parent;
};

template <class T>
bool BPlusTreeNode<T>::is_leaf(){
	for(unsigned int i=0; i<children.size(); i++){
		if(children[i]){
			return false;
		}
	}
	return true;
}

template <class T>
bool BPlusTreeNode<T>::contains(const T& key){
	return contains(key,0,keys.size()-1);
}

//Private method takes advantage of the fact keys are sorted
template <class T>
bool BPlusTreeNode<T>::contains(const T& key,std::size_t low,std::size_t high){
	if(low>high){
		return false;
	}
	if(low==high){
		return key == keys[low];
	}
	std::size_t mid = (low+high)/2;
	if(key<=keys[mid]){
		return contains(key,0,mid);
	}
	else{
		return contains(key,mid+1,high);
	}
}

///////////////////////////////////////////////////////////////////////////////////////
//Your BPlusTree implementation goes below this point.
///////////////////////////////////////////////////////////////////////////////////////

//If youre grading my code boy you're in for the ride of your life, get ready
template <class T>
class BPlusTree
{
	public:
	//both constructors, one the only takes in the b value, the other that copies another b+ tree
	BPlusTree(unsigned int a)
	{
		root = NULL;
		b = a;
	}
	BPlusTree(const BPlusTree<T>& original)
	{
		b= original.get_b();
		copy(original);
	}
	//the deconstructor
	~BPlusTree()
	{
		clear();
	}

	//various function headers, whose use will be explained below, and two functions to return the root and to return b
	void insert(const T& val);
	BPlusTreeNode<T>* find(const T& val) const;
	unsigned int get_b() const {return b;}
	BPlusTreeNode<T>* get_root() const {return root;}
	void print_sideways(std::ofstream& out) const;
	void print_BFS(std::ofstream& out) const;
	
	
	private:
	BPlusTreeNode<T>* root;
	unsigned int b;
	
	//functions used in the deconstuctor
	void clear();
	void clear2(BPlusTreeNode<T>* start);
	
	//functions used in the copy constructor
	void copy(const BPlusTree<T>& original);
	void copy2(BPlusTreeNode<T>* origianl,BPlusTreeNode<T>* current, BPlusTreeNode<T>* paren);
	
	//private helper functions
	void print_sideways2(std::ofstream& out,BPlusTreeNode<T>* node, int level) const;
	void insert2(const T& val, BPlusTreeNode<T>* nood);
	BPlusTreeNode<T>* find2(const T& val, BPlusTreeNode<T>* root) const;	
	void splitChild(BPlusTreeNode<T>* nood);
	void vectorfy(std::vector<std::vector<std::string> >& holder, BPlusTreeNode<T>* node,int level) const;
	
	
	
	
	
};



//function to print the tree sideways, only takes in an of stream object
template <class T> void BPlusTree<T>::print_sideways(std::ofstream& out) const
{
	if(root==NULL)
	{
		out << "Tree is empty."<<std::endl;
	}
	else
	{
		//the function that does all the work...
		print_sideways2(out,root,0);
	}
	
	
}

//function that does the work of printing a tree sideways
//takes in the of stream, a node that is origianlly the root, but is different at different levels of recursion
//lastly the level is an int that essentially keeps track of the necessary number of tabs
template <class T> void BPlusTree<T>::print_sideways2(std::ofstream& out,BPlusTreeNode<T>* node,int level) const
{
	//if the node is not a leaf
	if(node->children.size()!=0)
	{
		//go through the first half of the children nodes
		for(unsigned int i = 0;i<node->children.size()/2;i++)
		{
			print_sideways2(out,node->children[i],level+1);
		}
		//print out the keys of the node in the middle of the branches
		for(unsigned int i = 0;i<level;i++)
		{
			out<<"\t";
		}
		for(unsigned int i = 0;i<node->keys.size();i++)
		{
			out<<node->keys[i];
			if(i!=node->keys.size()-1)
			{
				out<<",";
			}
		}
		out<<std::endl;
		//go through the second half of the children nodes
		for(unsigned int i = node->children.size()/2;i<node->children.size();i++)
		{
			print_sideways2(out,node->children[i],level+1);
		}
	}
	//if the node is not a leaf
	else
	{
		//output everything in it
		for(unsigned int i = 0;i<level;i++)
		{
			out<<"\t";
		}
		for(unsigned int i = 0;i<node->keys.size();i++)
		{
			out<<node->keys[i];
			if(i!=node->keys.size()-1)
			{
				out<<",";
			}
		}
		out<<std::endl;
		
	}
}
//the print bfs function, takes in the ofstream as well
template <class T> void BPlusTree<T>::print_BFS(std::ofstream& out) const
{
	//prepare a vector that will be passed into the vectorfy function, which takes a 2d
	//vector and places every element in a tree at the level necessary
	std::vector<std::vector<std::string> > holder;
	std::vector<std::string> blank;
	holder.push_back(blank);
	//if the root is null, dont do anything, otherwise, use the vectorfy function to sort the data
	if(root==NULL)
	{
		out<<"Tree is empty."<<std::endl;
		return;
	}
	else
	{
		vectorfy(holder,root,0);
	}
	
	//use the vector given and output each vector of strings on its own line, representing its level
	for(unsigned int i = 0;i<holder.size();i++)
	{
		for(unsigned int j=0;j<holder[i].size();j++)
		{
			if(j>0)
			{
				out<<"\t";
			}
			out<<holder[i][j];
		}
		out<<std::endl;
	}
}

//the main function the drives the print bfs function
//takes in the 2d array by reference, a node that will allow the function to recursively go through the tree, and an int 
//to keep track of the level
template <class T> void BPlusTree<T>::vectorfy(std::vector<std::vector<std::string> >& holder, BPlusTreeNode<T>* node,int level) const
{
	//a string stream so that this function works for any data type
	std::string buffer;
	std::ostringstream temp;
	//take the keys at this level and turn it into a string before pushing it back on the vector
	 for(unsigned int i = 0;i<node->keys.size();i++)
	 {
		 if(i==node->keys.size()-1)
		 {
			temp<<(node->keys)[i];
		 }
		 else
		 {
			 temp<<(node->keys)[i]<<",";
		 }
	 }
	 //if you ever get to a new level, add a level to the vector
	 std::vector<std::string> blank;
	 while(holder.size()<level+1)
	 {
		 holder.push_back(blank);
	 }
	 holder[level].push_back(temp.str());
	 //goes through every child of the current node and vectorfys them
	 for(unsigned int i=0;i<node->children.size();i++)
	 {
		 vectorfy(holder,node->children[i],level+1);
	 }
}

//the driver for the copy function, used in the copy constructor
//takes an a tree that is to be copied as an argument
template <class T> void BPlusTree<T>::copy(const BPlusTree<T>& original)
{
	//create the root
	root = new BPlusTreeNode<T>;
	//copy the contents of the origianl tree
	copy2((original.get_root()),root,NULL);
}

//what does the copying in the copy function
//takes the original tree as an argument
//a node current that is used to keep track and recurse through the tree
//and a node paren that is used to assign the parent value to each node in the tree, essentially the node above the current node
template <class T> void BPlusTree<T>::copy2(BPlusTreeNode<T>* original, BPlusTreeNode<T>* current,BPlusTreeNode<T>* paren)
{
	//assigns the current nodes parent
	current->parent = paren;
	//copies over all the keys
	for(unsigned int i = 0;i<(original->keys).size();i++)
	{
		(current->keys).push_back((original->keys)[i]);
	}
	//copies over all the children, and then goes through them to continue the copying
	for(unsigned int i = 0;i<(original->children).size();i++)
	{
		BPlusTreeNode<T>* blank = new BPlusTreeNode<T>;
		(current->children).push_back(blank);
		copy2((original->children)[i],(current->children)[i],current);
	}
}

//fucntion primarily used in the deconstructor, takes no arguments
template <class T> void BPlusTree<T>::clear()
{
	if(root==NULL)
	{
		return;
	}
	//passes the root to the clear2 function
	clear2(root);
	root = NULL;
}

template <class T> void BPlusTree<T>::clear2(BPlusTreeNode<T>* start)
{

	//first recurses until it reaches the deepest level
	for(unsigned int i = 0;i<start->children.size();i++)
	{
		clear2(start->children[i]);
	}
	//then clears the data from each node
	start->children.clear();
	std::vector<BPlusTreeNode<T>*>(start->children).swap(start->children);
	start->keys.clear();
	std::vector<T>(start->keys).swap(start->keys);
	delete start;
	return;
}

//function to find a value if there is more than one node
//takes in the value to find and a node to start looking from
template <class T> BPlusTreeNode<T>* BPlusTree<T>::find2(const T& val,BPlusTreeNode<T>* root) const
{
	//variables to keep track of which child to go to
	unsigned int last_less;
	bool less_found = false;
	if(root==NULL)
	{
		return NULL;
	}
	//if you get to the end, return the current node
	else if(root->children.size()==0)
	{
		return root;
	}
	else
	{
		//find what child to go down
		for(unsigned int i = 0;i<root->keys.size();i++)
		{
			if((root->keys)[i]>val)
			{
				last_less = i;
				less_found = true;
				break;
			}
			
		}
		//go down that child 
		if(less_found)
		{
			return find2(val, root->children[last_less]);
		}
		else
		{
			return find2(val,root->children[(root->children).size()-1]);
		}
		
	}
	
	
	
}

//find function that also drives the find2 function,basically the same function as above
//takes in a value to find
template <class T> BPlusTreeNode<T>* BPlusTree<T>::find(const T& val) const
{
	unsigned int last_less;
	bool less_found = false;
	if(root==NULL)
	{
		return root;
	}
	else if(root->children.size()==0)
	{
		return root;
	}
	else
	{
		for(unsigned int i = 0;i<root->keys.size();i++)
		{
			if((root->keys)[i]>val)
			{
				last_less = i;
				less_found = true;
				break;
			}
			
		}
		if(less_found)
		{
			return find2(val, root->children[last_less]);
		}
		else
		{
			return find2(val,root->children[(root->children).size()-1]);
		}
		
	}
}

//the insert function, takes in a value to insert, works the same as the find function with a slight difference
//it puts the value in where it is supposed to go and calls the split child helper function
template <class T> void BPlusTree<T>::insert(const T& val)
{
	if(root==NULL)
	{
		root = new BPlusTreeNode<T>;
		root -> parent = NULL;
		root -> keys.push_back(val);
	}
	else
	{
		if(root->children.size()==0)
		{
			root->keys.push_back(val);
			std::sort(root->keys.begin(),root->keys.end());
			if((root->keys).size()==b)
			{
				//std::cout<<val<<" current"<<std::endl;
				splitChild(root);
			}
		}
		else
		{
			unsigned int last_less;
			bool less_found = false;
			for(unsigned int i = 0;i<root->keys.size();i++)
			{
				if((root->keys)[i]>val)
				{
					last_less = i;
					less_found = true;
					break;
				}
			
			}
			if(less_found)
			{
				insert2(val, root->children[last_less]);
			}
			else
			{
				insert2(val,root->children[(root->children).size()-1]);
			}
			
			
		}
			
	}
		
}


template <class T> void BPlusTree<T>::insert2(const T& val, BPlusTreeNode<T>* node)
{
	if(node==NULL)
	{
		node = new BPlusTreeNode<T>;
		node->parent = NULL;
		node->keys.push_back(val);
	}
	else if(node->children.size()==0)
	{
		node->keys.push_back(val);
		std::sort(node->keys.begin(),node->keys.end());
		if(node->keys.size()==b)
		{
			splitChild(node);
		}
	}
	else
	{
		unsigned int last_less;
		bool less_found = false;
		for(unsigned int i = 0;i<node->keys.size();i++)
		{
			if((node->keys)[i]>val)
			{
				last_less = i;
				less_found = true;
				break;
			}
			
		}
		if(less_found)
		{
			insert2(val, node->children[last_less]);
		}
		else
		{
			insert2(val,node->children[(node->children).size()-1]);
		}
	}
	
	
	
	
}


//the mother of all functions, takes a pointer to a node in the tree, handles the case where nodes need to be split
template <class T> void BPlusTree<T>::splitChild(BPlusTreeNode<T>* nood)
{
	//booleans to help aide in decision making
	bool new_parent=false;
	bool odd=false;
	bool leaf=false;
	unsigned int start;
	if(b%2!=0)
	{
		odd = true;
	}
	if(nood->children.size()==0)
	{
		leaf = true;
	}
	
	//values to find where the node will be split at and what value will go into the parent
	int split_at = (b/2);
	T split = (nood->keys)[split_at];
	//if the root is whats being split, this handles that
	if(nood->parent==NULL)
	{
		nood->parent = new BPlusTreeNode<T>();
		root = nood->parent;
		nood->parent->parent=NULL;
		new_parent = true;
	}
	//adds the necessary key to the parent
	(nood->parent->keys).push_back(split);
	std::sort(nood->parent->keys.begin(),nood->parent->keys.end());
	
	//creates the node that will be added to the parents children list
	BPlusTreeNode<T>* new_node = new BPlusTreeNode<T>;
	new_node->parent = nood->parent;
	
	//where the node will be split is partially dependent on if the node is a leaf or not, this handles that
	if(!leaf)
	{
		start = split_at+1;
	}
	else
	{
		start = split_at;
	}
	
	//take away half the children from the original node, and put them on the new node
	unsigned int counter = 0;
	for(unsigned int i =start;i<nood->children.size();i++)
	{
		new_node->children.push_back(nood->children[i]);
		counter++;
	}
	for(unsigned int i = 0;i<new_node->children.size();i++)
	{
		new_node->children[i]->parent=new_node;
	}
	for(unsigned int i = 0;i<counter;i++)
	{
		nood->children.pop_back();
	}
	
	
	//split up the keys between the two nodes
	for(unsigned int i = start;i<b;i++)
	{
		new_node->keys.push_back(nood->keys[i]);
	}
	for(unsigned int i = split_at;i<b;i++)
	{
		nood->keys.pop_back();
	}
	
	//if the root was split, then the original node will also have to be pushed back onto the new parent
	if(new_parent)
	{
		(nood->parent->children).push_back(nood);
	}
	
	//typedef used to insert the new node where it is supposed to go
	typedef typename std::vector<BPlusTreeNode<T>*>::iterator it;
	it insert_location = std::find(nood->parent->children.begin(),nood->parent->children.end(),nood);
	
	//put the new node in its place
	if(insert_location == nood->parent->children.end())
	{
		nood->parent->children.push_back(new_node);
	}
	else
	{
		(nood->parent->children).insert(insert_location+1,new_node);
	}
	
	//if in all this nonsense the parent is now to big, split that
	//this is recursive so it will go all the way up if necessary
	if((nood->parent->keys).size()==b)
	{
		splitChild(nood->parent);
	}
}
#endif
