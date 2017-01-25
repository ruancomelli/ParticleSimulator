// Standard
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <cstdio>
#include <iterator>
#include <algorithm>
#include <map>

// EntityLib
#include <Particle.h>
#include <SphericalParticle.h>
#include <SphericalParticlePtrArrayKit.h>

// UtilsLib
#include <Vector.h>
#include <Mathematics.h>
#include <Foreach.h>
#include <SharedPointer.h>

// ForceModelLib
#include <ForceModel.h>

// IOLib
#include <readEntity.h>
#include <readPhysicalEntity.h>
#include <readParticle.h>
#include <readSphericalParticle.h>
#include <FileReader.h>

// boost
#include <boost/math/constants/constants.hpp>

// System
#include <direct.h>

using namespace std;

using boost::math::constants::pi;

int main(int argc, char **argv){

	int defaultDimension = 3; // This means that we are constraint to Vector3D
	
	// Simulation data
	string inputPath("../_input/");
	FileReader simulationNameFile(inputPath + "input.txt");
	string simulationName;
	simulationNameFile.readValue("<simulationName>", simulationName);

	FileReader inputData(inputPath + simulationName + "/input.txt");
	double initialTime;
	double timeStep;
	double finalTime;
	int taylorOrder;
	int dimension;
	int numberOfParticles;
	int timeStepsForOutput;
	Vector3D gravity;

	inputData.readValue("<initialTime>", initialTime);
	inputData.readValue("<timeStep>", timeStep);
	inputData.readValue("<finalTime>", finalTime);
	inputData.readValue("<taylorOrder>", taylorOrder);
	inputData.readValue("<dimension>", dimension);
	inputData.readValue("<numberOfParticles>", numberOfParticles);
	inputData.readValue("<gravity>", gravity);
	inputData.readValue("<timeStepsForOutput>", timeStepsForOutput);

	string outputPath("../_output/" + simulationName + "/");

	_mkdir(outputPath.c_str());
	_mkdir((outputPath + "MATLAB_output/").c_str());

	// Input
	string particleInputPath(inputPath + simulationName + "/");

	SphericalParticlePtrArrayKit particleArray;

	particleArray.inputParticles(numberOfParticles, particleInputPath);


	foreach(SphericalParticlePtr particlePtr, particleArray){
		const double m = particlePtr->getScalarProperty( MASS );
		const double r = particlePtr->getScalarProperty( MASS );

		particlePtr->setScalarProperty( MOMENT_OF_INERTIA, 2 * m * r * r / 5 );
		particlePtr->setScalarProperty( VOLUME, 4 * pi<double>() * r * r * r / 3 );
	}

	particleArray[0]->setGravity(gravity);
	
	// Output
	particleArray.openFiles(outputPath);
	
	string verticalSeparator = "\n";
	string horizontalSeparator = ",";

	ofstream mainOutFile(outputPath + "output.txt");
	mainOutFile << "<nParticles> "		<< numberOfParticles	<< verticalSeparator;

	mainOutFile << "<initialTime> "		<< initialTime			<< verticalSeparator;
	mainOutFile << "<timeStep> "		<< timeStep				<< verticalSeparator;
	mainOutFile << "<finalTime> "		<< finalTime			<< verticalSeparator;

	mainOutFile << "<taylorOrder> "		<< taylorOrder			<< verticalSeparator;

	mainOutFile << "<timeStepsForOutput> "	<< timeStepsForOutput		<< verticalSeparator;

	particleArray.exportAllDataCSV();

	// ===== Simulation =====
	foreach( SphericalParticlePtr particle, particleArray ){
		foreach( SphericalParticlePtr neighbor, particleArray ){
			if( neighbor->getHandle() > particle->getHandle() ){
				particle->addNeighbor( neighbor );
			}
		}
	}

	int timeStepsForOutputCounter = 0;

	for(double t = initialTime; t <= finalTime ; t += timeStep){

		// Set forces and torques to zero
		foreach( SphericalParticlePtr particle, particleArray ){
			particle->setContactForce( nullVector3D() );
			particle->setBodyForce( nullVector3D() );
			particle->setResultingTorque( nullVector3D() );
		}

		// Body forces
		foreach( SphericalParticlePtr particle, particleArray ){
			particle->addBodyForce(particle->getScalarProperty(MASS) * gravity);
		}

		// Predict position and orientation
		foreach( SphericalParticlePtr particle, particleArray ){
			particle->setPosition( ForceModel::taylorPredictor( particle->getPosition(), taylorOrder, timeStep ) );
			particle->setOrientation( ForceModel::taylorPredictor( particle->getOrientation(), taylorOrder, timeStep ) );
		}

		// Contact forces
		
		foreach( SphericalParticlePtr particle, particleArray ){
			foreach( int handle, particle->getNeighborhood() ){
				SphericalParticlePtr neighbor = particleArray[handle];

				if(particle->touches(neighbor))	// If particles are in touch
				{
					ForceModel::viscoelasticSpheres( particle, neighbor );
				}
			}
		}

		// Correct position and orientation
		foreach( SphericalParticlePtr particle, particleArray ){
			ForceModel::correctPosition( particle , taylorOrder, timeStep );
			ForceModel::correctOrientation( particle , taylorOrder, timeStep );
		}


		// ----- Saving to file -----

		// Prints every derivative of particles' position
		if( (++timeStepsForOutputCounter) == timeStepsForOutput){
			timeStepsForOutputCounter = 0;

			particleArray.exportTemporalDataCSV();
		}
	}

	mainOutFile.close();
	
	cout << endl << "Success" << endl << endl;
}
