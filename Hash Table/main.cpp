#include "Hash.h"
#include <fstream>


int main()
{
	//temp is the variable that will be used to input from the file
	std::string temp;
	std::cin>>temp;
	//variables to determine if a default value for the size and occupancy will be needed
	bool size_found = false;
	bool occ_found = false;
	
	//a vector storing all the queries as pairs of the string and the number of mismatches allowed
	std::vector<std::pair<std::string,int> > query;
	
	//values that will be taken in from the file
	unsigned int table_size;
	double occupancy;
	int k;
	std::string fname;
	
	//basic file processing
	while(temp!="quit")
	{
		if(temp=="genome")
		{
			std::cin>>temp;
			fname = temp;
		}
		else if(temp=="table_size")
		{
			std::cin>>table_size;
			size_found=true;
		}
		else if(temp=="occupancy")
		{
			std::cin>>occupancy;
			occ_found = true;
		}
		else if(temp == "kmer")
		{
			std::cin>>k;
		}
		//creating the list of queries
		else if(temp=="query")
		{
			std::string query2;
			int mismatch2;
			std::cin>>mismatch2>>query2;
			std::pair<std::string, int> temporary(query2,mismatch2);
			query.push_back(temporary);
		}
		std::cin>>temp;
	}
	//default assignment of values if they were not found in the file
	if(!occ_found)
	{
		occupancy  = .5;
	}
	if(!size_found)
	{
		table_size=100;
	}
	
	//open the file
	std::ifstream in(fname.c_str());
	
	//create the hash table object
	Hash table(table_size,occupancy,k);
	
	
	//read in the genome from the file
	std::string genome ="";
	while(in>>temp)
	{
		genome+=temp;
	}
	
	
	//create all the kmers
	for(unsigned int i = 0;i<genome.size()-1-k;i++)
	{
		table.insert(genome.substr(i,k),i);
	}
	
	
	//go through and perform all the queries
	for(unsigned int i = 0;i<query.size();i++)
	{
		table.find(query[i].first,query[i].second,genome);
	}
	
	return 0;
}