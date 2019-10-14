#ifndef Hash_h
#define Hash_h

#include <vector>
#include <string>
#include <iostream>

//the node class that is used to make the singly linked lists for the hash table
class Node
{
	//this class stores a pointer to the next element in the list, a string value, and a vector of the locations the string is found
	public:
		Node()
		{
			value ="";
		}
		std::string value;
		std::vector<unsigned int> locations;
};

//the main class
class Hash
{
	//public functions, greater detail on them in the cpp file
	public:
	Hash(unsigned int size2,float occupancy2,unsigned int kmer);
	void insert(std::string const& k,unsigned int position);
	void find(std::string const& sequence, unsigned int diff,const std::string& genome);
	
	
	private:
	//the class stores a size value, a max occupancy value, the length of the kmers inside the table, the table itself
	//and the number of unique keys
	unsigned int size;
	float occupancy;
	unsigned int k;
	std::vector<Node> table;
	unsigned int unique;
	
	//private functions, same as the public functions
	void resize();
	void insert2(const Node& node);
	
	unsigned long hash(std::string str);
	
};

//helper function
unsigned int diff_strings(const std::string& s1,const std::string& s2);

#endif