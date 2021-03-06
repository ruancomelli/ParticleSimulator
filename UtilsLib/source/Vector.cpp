#include <Vector.hpp>

// Standard
#include <algorithm>
#include <cmath>
#include <numeric>
#include <stdexcept>

namespace psin {

using std::runtime_error;

// DoubleVectorOperations

// Scalar by vector multiplication
DoubleVector operator *(const double & scalar, DoubleVector vec){
	for( double& entry : vec)
	{
		entry *= scalar;
	}

	return vec;
}
// Vector by scalar multiplication
DoubleVector operator *(const DoubleVector & vec, const double & scalar){
	return scalar * vec;
}

DoubleVector operator -=( DoubleVector& v, const double& d ){
	for( double& entry : v ){
		entry -= d;
	}
	return v;
}

DoubleVector operator +=( DoubleVector& v, const double& d ){
	for( double& entry : v ){
		entry += d;
	}
	return v;
}

double add( double a, double b ){
	return a + b;
}

double subtract( double a, double b ){
	return a - b;
}

void operator +=( DoubleVector& vector1, const DoubleVector& vector2 ){
	if( vector1.size() != vector2.size() ) {
		throw runtime_error( "DoubleVector += operator: Vectors dimensions don't agree." );
	}
	std::transform( vector1.begin(), vector1.end(), vector2.begin(), vector1.begin(), add );
}

void operator -=( DoubleVector& vector1, const DoubleVector& vector2 ){
	if( vector1.size() != vector2.size() ) {
		throw runtime_error( "DoubleVector -= operator: Vectors dimensions don't agree." );
	}
	std::transform( vector1.begin(), vector1.end(), vector2.begin(), vector1.begin(), subtract );
}

void operator *=( DoubleVector& vec, const double& scalar ){
	for( double& entry : vec ){
		entry *= scalar;
	}
}

double innerProduct( const DoubleVector& vector1, const DoubleVector& vector2 ){
	return std::inner_product( vector1.begin(), vector1.end(), vector2.begin(), 0.0 );
}

double norm( DoubleVector& vec ){
	return sqrt( innerProduct( vec, vec ) );
}

void normalize( DoubleVector& vec ){
	vec *= 1.0/norm( vec );
}

// DoubleVector2D operations
void operator +=( DoubleVector2D& mat1, const DoubleVector2D& mat2 )
{
	if( (mat1.size() != mat2.size()) or ( mat1[0].size() != mat2[0].size() ) ) 
	{
		throw runtime_error( "DoubleVector2D += operator: matrices dimensions don't agree." );
	}
	
	for( unsigned i = 0 ; i < mat1.size() ; ++i )
	{
		std::transform( mat1[i].begin(), mat1[i].end(), mat2[i].begin(), mat1[i].begin(), add );
	}
}

DoubleVector operator *( const DoubleVector2D & A, const DoubleVector & vector ){
	/* It performs the matrix product of a matrix and a vector -> solution = A.x */
	int nColsA = A[0].size();
	int nRowsA = A.size();
	int nRowsV = vector.size();
	if( nColsA != nRowsV ) {	throw runtime_error( "Matrix and vector dimensions do not match." );		}
	DoubleVector solution( nRowsV, 0.0 );
	for( int i = 0; i < nRowsA ; ++i ){
		double sum = 0.0;
		for( int j = 0; j < nColsA ; ++j ){
			sum += A[i][j]*vector[j];
		}
		solution[i] = sum;
	}
	return solution;
}

DoubleVector2D operator *( const double scalar, DoubleVector2D matrix ){
	for( auto& line : matrix )
	{
		for( auto& entry : line )
		{
			entry *= scalar;
		}
	}

	return matrix;
}

DoubleVector2D operator *( const DoubleVector2D & A, const DoubleVector2D & B ){
	/* It performs the matrix product of A and B -> prod_A_B(A,B) = A.B */
	int nColsA = A[0].size();		int nRowsA = A.size();
	int nColsB = B[0].size();		int nRowsB = B.size();

	if( nColsA != nRowsB ) {	throw runtime_error( "Matrices dimensions do not match." );		}

	DoubleVector2D solution( nRowsA, DoubleVector( nColsB, 0.0 ) );

	for( int i = 0; i < nRowsA ; ++i ){
		for( int j = 0; j < nColsB ; ++j ){
			double sum = 0.0;
			for( int k = 0 ; k < nColsA; ++k){
				sum += A[i][k] * B[k][j];
			}
			solution[i][j] = sum;
		}
	}
	return solution;
}

DoubleVector nullVector(int dimension){
	return DoubleVector(dimension, 0);
}

} // psin