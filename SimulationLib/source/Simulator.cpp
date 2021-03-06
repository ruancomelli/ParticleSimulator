// #include <Simulator.hpp>

// // IOLib
// #include <FileReader.hpp>

// // PropertyLib
// #include <PropertyDefinitions.hpp>

// // UtilsLib
// #include <FileSystem.hpp>
// #include <ProgramOptions.hpp>

// // ----- Default simulation -----
// void Simulator::defaultSimulate(const string simulationName, const string projectRootFolder)
// {
// 	this->setName(simulationName);
// 	this->fileTree.setTree(simulationName, projectRootFolder);

// 	this->inputMainData();

// 	this->initializeParticleArray();

// 	this->outputMainData();

// 	this->simulate();

// 	this->printSuccessMessage();
// }

// // Simulate
// void Simulator::simulate(void)
// {
// 	// Output
// 	particleArray.exportAllDataCSV();
// 	this->fileTree.timeVectorForPlotFile << 0 << "\n";

// 	// ===== Simulator =====

// 	int stepsForStoringCounter = 0;

// 	for(double t = initialInstant; t <= finalInstant ; t += timeStep){

// 		this->fileTree.timeVectorFile << t << "\n";

// 		// Set forces and torques to zero
// 		for( SphericalParticlePtr particle : particleArray ){
// 			particle->setContactForce( nullVector3D() );
// 			particle->setBodyForce( nullVector3D() );
// 			particle->setResultingTorque( nullVector3D() );
// 		}

// 		// Body forces
// 		for( SphericalParticlePtr particle : particleArray ){
// 			particle->addBodyForce(particle->get( mass ) * gravity);
// 		}

// 		// Predict position and orientation
// 		for( SphericalParticlePtr particle : particleArray ){
// 			particle->setPosition( Interaction<SphericalParticle, SphericalParticle>::taylorPredictor( particle->getPosition(), taylorOrder, timeStep ) );
// 			particle->setOrientation( Interaction<SphericalParticle, SphericalParticle>::taylorPredictor( particle->getOrientation(), taylorOrder, timeStep ) );
// 		}

// 		// Contact forces
		
// 		for( SphericalParticlePtr particle : particleArray ){
// 			for( int handle : particle->getNeighborhood() ){
// 				SphericalParticlePtr neighbor = particleArray[handle];

// 				Interaction.calculate(particle, neighbor);
// 			}
// 		}

// 		// Correct position and orientation
// 		for( SphericalParticlePtr particle : particleArray ){
// 			Interaction<SphericalParticle, SphericalParticle>::correctPosition( particle , taylorOrder, timeStep );
// 			Interaction<SphericalParticle, SphericalParticle>::correctOrientation( particle , taylorOrder, timeStep );
// 		}


// 		// ----- Saving to file -----

// 		// Prints every derivative of particles' position
// 		if( (++stepsForStoringCounter) == stepsForStoring){
// 			stepsForStoringCounter = 0;

// 			particleArray.exportTemporalDataCSV();

// 			this->fileTree.timeVectorForPlotFile << t << "\n";
// 		}
// 	}
// }

// // ----- Input -----
// void Simulator::inputMainData(void)
// {
// 	FileReader inputData(this->fileTree.getInputMainDataFilePath());

// 	// Read simulation data
// 	inputData.readValue("<initialInstant>", this->initialInstant);
// 	inputData.readValue("<timeStep>", this->timeStep);
// 	inputData.readValue("<finalInstant>", this->finalInstant);
// 	inputData.readValue("<taylorOrder>", this->taylorOrder);
// 	inputData.readValue("<dimension>", this->dimension);
// 	inputData.readValue("<numberOfParticles>", this->numberOfParticles);
// 	inputData.readValue("<gravity>", this->gravity);
// 	inputData.readValue("<stepsForStoring>", this->stepsForStoring);

// 	// Read Interaction
// 	std::string InteractionName;
// 	inputData.readValue("<InteractionName>", InteractionName);

// 	this->setInteraction(InteractionName);
// }

// // ----- Output -----
// void Simulator::outputMainData(void) const
// {
// 	std::string verticalSeparator = "\n";
// 	std::string horizontalSeparator = ",";

// 	std::ofstream mainOutFile(this->fileTree.getNumericalOutputFolder() + "mainInfoOutput.txt");

// 	mainOutFile << "<nParticles> "		<< numberOfParticles	<< verticalSeparator;
// 	mainOutFile << "<initialInstant> "		<< initialInstant			<< verticalSeparator;
// 	mainOutFile << "<timeStep> "		<< timeStep				<< verticalSeparator;
// 	mainOutFile << "<finalInstant> "		<< finalInstant			<< verticalSeparator;
// 	mainOutFile << "<taylorOrder> "		<< taylorOrder			<< verticalSeparator;
// 	mainOutFile << "<stepsForStoring> "	<< stepsForStoring		<< verticalSeparator;
// 	mainOutFile << "<InteractionName> " << Interaction.getName() << verticalSeparator;
// }


// // Particles
// void Simulator::initializeParticleArray(void)
// {
// 	// Input
// 	this->particleArray.requirePropertyContainer(this->Interaction.getRequiredProperties());

// 	this->particleArray.inputParticles(this->numberOfParticles, this->fileTree.getParticleInputFolder());

// 	for(auto& particlePtr : particleArray){
// 		double m = particlePtr->get( mass );
// 		double r = particlePtr->getGeometricParameter( RADIUS );

// 		particlePtr->set( moment_of_inertia, 2 * m * r * r / 5 );
// 	}

// 	particleArray[0]->setGravity(this->gravity);
// 	particleArray.openFiles(this->fileTree.getNumericalOutputFolder());

// 	Interaction<SphericalParticle, SphericalParticle>::setNumberOfParticles( this->numberOfParticles );

// 	for( SphericalParticlePtr particle : particleArray ){
// 		for( SphericalParticlePtr neighbor : particleArray ){
// 			if( neighbor->getHandle() > particle->getHandle() ){
// 				particle->addNeighbor( neighbor );
// 			}
// 		}
// 	}
// }

// // Interaction
// void Simulator::setInteraction(const std::string & InteractionName)
// {
// 	for (auto& fm : this->InteractionSet)
// 	{
// 		if (fm.getName() == InteractionName)
// 		{
// 			this->Interaction = fm;
// 			break;
// 		}
// 	}
// }

// void Simulator::appendInteraction( const Interaction<SphericalParticle, SphericalParticle> & fm ){
// 	this->InteractionSet.insert(fm);
// }

// // Ending simulation
// void Simulator::printSuccessMessage(void) const
// {
// 	std::cout << std::endl << "Success" << std::endl << std::endl;
// }