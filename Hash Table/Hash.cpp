#include "Hash.h"

//The constuctor, takes size and occupancy as well as the size of the kmers stored
Hash::Hash(unsigned int size2,float occupancy2,unsigned int kmer)
{
	size = size2;
	occupancy = occupancy2;
	k = kmer;
	unique = 0;
	for(unsigned int i = 0;i<size;i++)
	{
		table.push_back(Node());
	}
}

//the insert function for the resize function
void Hash::insert2(const Node& node)
{
	//finds where the node should be placed
	unsigned int index = hash(node.value);
	//if that spot is not taken, put it there
	if(table[index].value=="")
	{
		table[index] = node;
	}
	//otherwise find the next available space
	else
	{
		while(true)
		{
			index = (index+1)%size;
			if(table[index].value=="")
			{
				table[index]=node;
				break;
			}
		}
	}
}


//the function that resizes the hash table
void Hash::resize()
{
	//put all the nodes that are in the hash table into a temporary vector
	std::vector<Node> temp;
	for(unsigned int i = 0;i<size;i++)
	{
		if(table[i].value!="")
		{
			temp.push_back(table[i]);
			table[i]=Node();
		}
	}
	//then double the size of the table
	for(unsigned int i = 0;i<size;i++)
	{
		table.push_back(Node());
	}
	size = size + size;
	//then put all the nodes back into the table
	for(unsigned int i = 0;i<temp.size();i++)
	{
		insert2(temp[i]);
	}
}



//the hash funciton,taken from the lecture with an added bit shift because the one we got in class sucked
unsigned long Hash::hash(std::string str) 
{
	unsigned int value = 0;
	for(unsigned int i = 0;i<str.size();i++)
	{
		value = value*8+str[i];
		value = (value<<4)^value;
	}
	return value%size;
}



//the insert function,takes a string and the position in the genome that string was found
void Hash::insert(std::string const& k,unsigned int position)
{
	//if the index is empty, put the kmer there
	unsigned int index = hash(k);
	if(table[index].value=="")
	{
		table[index].value = k;
		table[index].locations.push_back(position);
		unique+=1;
	}
	//otherwise, if the kmer is at the location, push back the position
	else if(table[index].value==k)
	{
		table[index].locations.push_back(position);
	}
	//otherwise, find the location where the kmer is, or the next free spot and do the same as above
	else
	{
		unsigned int current = index;
		current = (current+1)%size;
		while(current!=index)
		{
			
			if(table[current].value==k)
			{
				table[current].locations.push_back(position);
				break;
			}
			else if(table[current].value=="")
			{
				table[current].value=k;
				table[current].locations.push_back(position);
				unique+=1;
				break;
			}
			current = (current+1)%size;
		}
		
	}
	
	//if the hash table is becoming too full, resize it
	if( (unique/size)>occupancy)
	{
		resize();
	}
}

//a helper function to find the number of differences in two strings
unsigned int diff_strings(const std::string& s1,const std::string& s2)
{
	//if they are different sizes, return a large number to indicate some error
	unsigned int total = 0;
	if(s1.size()!=s2.size())
	{
		return 9999;
	}
	
	//otherwise find the number of differences and return that number
	for(unsigned int i = 0;i<s1.size();i++)
	{
		if(s1[i]!=s2[i])
		{
			total++;
		}
	}
	return total;
}


//finds the location of a string in the hash table
void Hash::find(std::string const& sequence, unsigned int diff,const std::string& genome)
{
	
	std::cout<< "Query: "<<sequence<<std::endl;
	//this works in essentially the same way, but if a blank space is ever encountered it means the kmer is not in the table
	unsigned int found_index;
	bool found = false;
	std::string kmer = sequence.substr(0,k);
	unsigned int index = hash(kmer);
	if(table[index].value=="")
	{
		std::cout<<"No Match"<<std::endl;
		return;
	}
	else if(table[index].value==kmer)
	{
		found_index = index;
	}
	else
	{
		found_index = index;
		while(true)
		{
			found_index = (found_index+1)%size;
			if(table[found_index].value==kmer)
			{
				break;
			}
			else if(found_index==index|| table[found_index].value=="")
			{
				std::cout<<"No Match"<<std::endl;
				return;
			}
		}
	}
	
	//if the node is found get its position vector
	std::vector<unsigned int> position(table[found_index].locations);
	for(unsigned int i = 0;i<position.size();i++)
	{
		//if any of the strings located at the positions in the vector match within the specified error, print them out
		std::string temp = genome.substr(position[i],sequence.length());
		if(diff_strings(temp,sequence)<=diff)
		{
			found = true;
			std::cout<<position[i]<<" "<<diff_strings(sequence,temp)<<" "<< temp<<std::endl;
		}
	}
	//if no correct string were found, print this message
	if(!found)
	{
		std::cout<<"No Match"<<std::endl;
	}
}








