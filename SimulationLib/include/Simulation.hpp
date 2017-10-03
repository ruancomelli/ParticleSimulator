#ifndef SIMULATION_HPP
#define SIMULATION_HPP

// InteractionLib
#include <Interaction.hpp>

// IOLib
#include <SphericalParticlePtrArrayKit.hpp>

// SimulationLib
#include <InteractionSubjectLister.hpp>
#include <LooperDefinitions.hpp>
#include <SeekerDefinitions.hpp>
#include <SimulationFileTree.hpp>

// UtilsLib
#include <FileSystem.hpp>
#include <Named.hpp>
#include <Vector3D.hpp>
#include <string.hpp>

// Standard
#include <tuple>

namespace psin
{

template<typename ... ParticleTypes> using ParticleList = mp::type_list<ParticleTypes...>;
template<typename ... BoundaryTypes> using BoundaryList = mp::type_list<BoundaryTypes...>;
template<typename ... InteractionTypes> using InteractionList = mp::type_list<InteractionTypes...>;
template<typename ... LooperTypes> using LooperList = mp::type_list<LooperTypes...>;
template<typename ... SeekerTypes> using SeekerList = mp::type_list<SeekerTypes...>;

template<
	typename Particles,
	typename Boundaries,
	typename Interactions,
	typename Loopers,
	typename Seekers
> 
class Simulation;

template<
	typename ... ParticleTypes,
	typename ... BoundaryTypes,
	typename ... InteractionTypes
>
class Simulation<
	ParticleList<ParticleTypes...>,
	BoundaryList<BoundaryTypes...>,
	InteractionList<InteractionTypes...>,
	LooperList<GearLooper>,
	SeekerList<BlindSeeker>
> : public Named
{
public:
	using ParticleList = psin::ParticleList<ParticleTypes...>;
	using BoundaryList = psin::BoundaryList<BoundaryTypes...>;
	using InteractionList = psin::InteractionList<InteractionTypes...>;
	using LooperList = psin::LooperList<GearLooper>;
	using SeekerList = psin::SeekerList<BlindSeeker>;
	using InteractionParticleParticleTriplets = typename InteractionSubjectLister::generate_combinations<InteractionList, ParticleList, ParticleList>::type;
	using InteractionParticleBoundaryTriplets = typename InteractionSubjectLister::generate_combinations<InteractionList, ParticleList, BoundaryList>::type;

	void setup(const path & mainInputFilePath);

	void setupInteractions(const json & interactionsJSON);
	void buildParticles(const json & particlesJSON);
	void buildBoundaries(const json & boundariesJSON);

	void openFiles();

	// Output
	void outputMainData() const;
	void createDirectories() const;
	void backupInteractions() const;
	void backupParticles() const;
	void backupBoundaries() const;

	template<typename Time> void exportParticles(const Time & time);
	template<typename Time> void exportBoundaries(const Time & time);

	void printSuccessMessage() const;

	// Simulate
	void simulate();

private:
	json fileTree;
	
	std::map<string, unique_ptr<std::fstream>> mainFileMap;
	std::map<string, unique_ptr<std::fstream>> particleFileMap;
	std::map<string, unique_ptr<std::fstream>> boundaryFileMap;

	std::map<string, vector<json>> particleJsonMap;
	std::map<string, vector<json>> boundaryJsonMap;

	double initialTime;
	double timeStep;
	double finalTime;
	int taylorOrder;
	int dimension;
	unsigned long timeStepsForOutput;
	unsigned long outputsForExporting;

	std::tuple< std::vector<ParticleTypes>... > particles;
	std::tuple< std::vector<BoundaryTypes>... > boundaries;

	std::set< std::string > interactionsToUse;
	string looperToUse;
	string seekerToUse;
};

} // psin

#include <Simulation.tpp>

#endif
