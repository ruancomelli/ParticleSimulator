#define BOOST_TEST_MODULE EntityLibTest

// UtilsLib
#include <Mathematics.h>
#include <Vector3D.h>
#include <Vector.h>
#include <Test.h>
#include <Foreach.h>

// EntityLib
#include <Entity.h>
#include <PhysicalEntity.h>

using namespace std;

TestCase( EntityTest )
{
	Entity entity1;

	// Test the default handle and the getHandle function
	int defaultHandle = -1;
	checkEqual( entity1.getHandle() , defaultHandle );

	// Test the setHandle function
	int handleToSet = 2;
	entity1.setHandle(handleToSet);
	checkEqual( entity1.getHandle() , handleToSet );
	// Test constructor passing one argument
	int initialHandle = 1;
	Entity entity2(initialHandle);
	checkEqual( entity2.getHandle() , initialHandle );
}

TestCase( PhysicalEntityDefaultConstructor )
{
	// Default Constructor
	PhysicalEntity 		physicalEntity;
	int 				defaultTaylorOrder 	= 3;
	int 				defaultSize 		= defaultTaylorOrder + 1;
	int 				defaultDimension 	= 3;
	int					defaultHandle		= -1;

	vector<Vector3D> 			defaultPosition 	= physicalEntity.getPosition();
	vector<Vector3D> 			defaultOrientation 	= physicalEntity.getOrientation();
	DoubleVector				scalarProperty		= physicalEntity.getScalarProperty();
	DoubleVector2D				vectorialProperty	= physicalEntity.getVectorialProperty();
	vector < DoubleVector2D >	matricialProperty	= physicalEntity.getMatricialProperty();
	
	checkEqual( physicalEntity.getHandle() , defaultHandle );
	checkEqual( defaultPosition.size() , defaultSize );
	checkEqual( defaultOrientation.size() , defaultSize );
	checkEqual( physicalEntity.getDimension() , defaultDimension );
	checkEqual( scalarProperty.size() , N_SCALAR_PROPERTY );
	checkEqual( vectorialProperty.size() , N_VECTORIAL_PROPERTY );
	checkEqual( matricialProperty.size() , N_MATRICIAL_PROPERTY );
}

TestCase( PhysicalEntityConstructorWithParametes )
{
	int taylorOrder	= 2;
	int size		= taylorOrder + 1;
	int dimension	= 2;
	int handle		= 5;

	// Constructor with non-default parameters
	// Properties allocator not tested
	PhysicalEntity physicalEntity(taylorOrder, dimension, handle);

	vector<Vector3D> 	position 		= physicalEntity.getPosition();
	vector<Vector3D> 	orientation 	= physicalEntity.getOrientation();

	checkEqual( physicalEntity.getHandle() , handle );
	checkEqual( position.size() , size );
	checkEqual( orientation.size() , size );
	checkEqual( physicalEntity.getDimension() , dimension );
	
}

TestCase( SpacialSetFunctions )
{

	int taylorOrder	= 1;
	int dimension	= 3;
	int handle		= 0;
	PhysicalEntity physicalEntity(taylorOrder, dimension, handle);

	// setPosition(derivative, x, y, z)
	{
	double x = -5.4;
	double y = 8.3;
	double z = -1.7;
	int derivative = 0;

	Vector3D pos0(x , y , z);
	physicalEntity.setPosition(derivative, x, y, z);
	check( pos0==physicalEntity.getPosition(0) );
	}

	// setPosition(derivative, vector3D)
	{
	double x = -8.6;
	double y = 4.9;
	double z = -6.8;
	int derivative = 1;

	Vector3D pos0(x , y , z);
	physicalEntity.setPosition(derivative, pos0);
	check( pos0==physicalEntity.getPosition(derivative) );
	}

	// setPosition( vector<Vector3D> )
	{
	Vector3D pos0( 1.0 , -3.7 , -7.4 );
	Vector3D pos1( 6.2 , 1.9 , -2.8 );
	vector<Vector3D> posVec (2);
	posVec[0] = pos0;
	posVec[1] = pos1;

	physicalEntity.setPosition(posVec);

	check( pos0==physicalEntity.getPosition()[0] );
	check( pos1==physicalEntity.getPosition()[1] );
	}
}

TestCase( PropertiesTest )
{
	PhysicalEntity	physicalEntity;
	int taylorOrder	= 4;
	int size		= taylorOrder + 1;

	// setTaylorOrder
	physicalEntity.setTaylorOrder(taylorOrder);
	checkEqual( physicalEntity.getTaylorOrder() , taylorOrder );
	checkEqual( physicalEntity.getPosition().size() , size );

	// setScalarProperty(id, value);
	double elasticModulus = 14.95;
	physicalEntity.setScalarProperty( ELASTIC_MODULUS , elasticModulus );
	checkEqual( physicalEntity.getScalarProperty(ELASTIC_MODULUS) , elasticModulus );

	// setScalarProperty(vector)
	double dissipativeConstant = 8.43;
	double frictionParameter = 1.67;
	DoubleVector scalarProperties( N_SCALAR_PROPERTY );

	scalarProperties[DISSIPATIVE_CONSTANT] = dissipativeConstant;
	scalarProperties[FRICTION_PARAMETER] = frictionParameter;

	physicalEntity.setScalarProperty(scalarProperties);

	checkClose( physicalEntity.getScalarProperty()[DISSIPATIVE_CONSTANT] , dissipativeConstant , 1.0e-12 );
	checkClose( physicalEntity.getScalarProperty()[FRICTION_PARAMETER] , frictionParameter , 1.0e-12 );
	checkClose( physicalEntity.getScalarProperty()[VOLUME] , 0.0 , 1.0e-12 );
}

TestCase( PhysicalEntityDistance )
{
	PhysicalEntity phy[2];

	double x[2], y[2], z[2];

	x[0] = -3.7;
	y[0] = 4.9;
	z[0] = 5.31;
	phy[0].setPosition( 0 , x[0] , y[0] , z[0] );

	x[1] = 3.5;
	y[1] = -6.9;
	z[1] = 0.17;
	phy[1].setPosition( 0 , x[1] , y[1] , z[1] );

	double distance = 14.7478676424763;
	checkClose( phy[0].distance(phy[1]) , distance , 1e-12 );
}
