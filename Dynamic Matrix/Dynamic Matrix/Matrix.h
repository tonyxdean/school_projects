#ifndef Matrix_h_
#define Matrix_h_

#include <iostream>
#include <math.h>

class Matrix
{
	public:
	//constructors and destuctor
	Matrix(unsigned int n_rows,unsigned int n_col,double fill);
	Matrix();
	Matrix(const Matrix& m);
	~Matrix();
	
	//returns and sets the values in the matrix
	unsigned int num_rows() const;
	unsigned int num_cols() const;
	bool get(unsigned int arow, unsigned int acolumn,double& dub) const; //
	bool set(unsigned int arow,unsigned int acolumn, double dub); //
	
	
	//the various functions necessary to complete the assignment
	void multiply_by_coefficient(double coef); //
	bool swap_row(unsigned int first,unsigned int second); //
	void transpose(); //
	bool add(const Matrix& m); //
	bool subtract(const Matrix& m); //
	double* get_row(unsigned int row) const; //
	double* get_col(unsigned int col) const; //
	Matrix* quarter() const; //
	
	//the various helper functions
	void clear(); //

	
	//the resize function
	void resize(unsigned int num_row,unsigned int num_col,double fill);
	
	//the operators
	bool operator==(const Matrix& m); //
	bool operator!=(const Matrix& m); //
	Matrix& operator=(const Matrix& m); //
	
	
	
	private:

	//the data stored in the class
	unsigned int rows;
	unsigned int columns;
	double** data;

	//private helper functions
	void create(unsigned int n_rows,unsigned int n_col,double fill);
	void copy(const Matrix& m); //

		
};



std::ostream& operator<<(std::ostream& out,const Matrix& m);


#endif