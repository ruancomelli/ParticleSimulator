// Standard
#include <string>
#include <vector>

// ForceModelLib
#include <ForceMethodsDefinitions.h>

// PropertyLib
#include <PropertyDefinitions.h>

// SimulationLib
#include <Simulation.h>

using namespace std;
using namespace PropertyDefinitions;

const string project_root_path = PROJECT_PATH;

int main(int argc, char **argv){
	pair<string, string> myPair = Simulation::getSimulationNameAndRootPath(argc, argv);
	string simulationName = myPair.first;
	string rootPath = myPair.second + "\\";

	Simulation simulation;

	ForceModel viscoelasticSpheres("ViscoElasticSpheres");
	viscoelasticSpheres.setNormal(normalForceViscoelasticSpheres);
	viscoelasticSpheres.setTangential(tangentialForceHaffWerner);
	viscoelasticSpheres.requireProperty(mass);
	viscoelasticSpheres.requireProperty(moment_of_inertia);
	viscoelasticSpheres.requireProperty(elastic_modulus);
	viscoelasticSpheres.requireProperty(dissipative_constant);
	viscoelasticSpheres.requireProperty(poisson_ratio);
	viscoelasticSpheres.requireProperty(tangential_damping);
	viscoelasticSpheres.requireProperty(friction_parameter);

	simulation.addForceModel(viscoelasticSpheres);


	ForceModel electrostatic("Electrostatic");
	electrostatic.setField(electrostaticForce);
	electrostatic.setNormal(normalForceViscoelasticSpheres);
	electrostatic.setTangential(tangentialForceHaffWerner);
	electrostatic.requireProperty(mass);
	electrostatic.requireProperty(moment_of_inertia);
	electrostatic.requireProperty(elastic_modulus);
	electrostatic.requireProperty(dissipative_constant);
	electrostatic.requireProperty(poisson_ratio);
	electrostatic.requireProperty(tangential_damping);
	electrostatic.requireProperty(friction_parameter);
	electrostatic.requireProperty(electric_charge);

	simulation.addForceModel(electrostatic);

	simulation.defaultSimulate(simulationName, rootPath);
}
