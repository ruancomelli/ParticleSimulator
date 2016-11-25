#include <readPhysicalEntity.h>

PhysicalEntity readPhysicalEntity( string & fileName )
{
	FileReader fileReader(fileName);

	// ----- Read Entity -----
	Entity entity = readEntity(fileName);
	

	// ----- Read taylorOrder -----
	int taylorOrder;
	fileReader.readValue("<TaylorOrder>", taylorOrder);

	// ----- Read dimension -----
	int dimension;
	fileReader.readValue("<Dimension>", dimension);

	// ----- Create object -----
	PhysicalEntity physicalEntity( taylorOrder, dimension, entity );


	// ----- Read initial position -----
	int size = taylorOrder + 1;


	vector<Vector3D> position;
	position.resize(size);


	return physicalEntity;
}