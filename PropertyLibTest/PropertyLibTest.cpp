#define BOOST_TEST_MODULE PropertyLibTest

// Standard
#include <iostream>
#include <fstream>
#include <iterator>

// PropertyLib
#include <Property.hpp>
#include <PropertyContainer.hpp>
#include <PropertyDefinitions.hpp>

// UtilsLib
#include <Test.hpp>

using namespace PropertyDefinitions;
using namespace std;

TestCase(PropertyConstructorsTest)
{
	double tolerance = 1e-12;
	double value = 3.14;

	// Test default constructor
	Property<double> property1;
	check(!property1.assigned());

	property1.set(value);
	check(property1.assigned());
	checkClose(property1.get(), value, tolerance);

	// Test Property<double>(const double & value)
	Property<double> property2(value);
	check(property2.assigned());
	checkClose(property2.get(), value, tolerance);
}

TestCase(PropertyInputAndOutputTest)
{
	double tolerance = 1e-12;
	double value = 3.14;

	string fileName = "Property_Input_Output_Test.txt";
	fstream file(fileName, fstream::in | fstream::out | fstream::trunc);

	Property<double> property1(value);
	property1.output(file);

	file.clear();
	file.seekg(0, ios::beg);

	Property<double> property2;
	property2.input(file);

	check(property2.assigned());
	checkClose(property2.get(), value, tolerance);
}

TestCase(PropertyDefinitionsTest)
{
	string massName = "Mass";
	double positiveValue = 3.14;
	double negativeValue = -8.5;
	double tolerance = 1e-12;

	checkEqual(Mass::name, massName);

	Mass mass;

	mass.set(positiveValue);
	check(mass.assigned());
	checkClose(mass.get(), positiveValue, tolerance);

	mass.set(negativeValue);
	check(!mass.assigned());
	checkClose(mass.get(), positiveValue, tolerance);

}

TestCase(PropertyContainerInstantiationTest)
{
	PropertyContainer<int, double, char> propertyContainerSuccess;

	//Uncomment the following line to get compile errors:
	//PropertyContainer<int, double, double> propertyContainerFail;
}

TestCase(PropertyContainerPropertyTest)
{
	int intValue = 3;
	double doubleValue = 5.6;

	PropertyContainer<int, double> propertyContainer;
	propertyContainer.property<int>() = intValue;
	propertyContainer.property<double>() = doubleValue;

	checkEqual(propertyContainer.property<int>(), intValue);
	checkEqual(propertyContainer.property<double>(), doubleValue);

	auto x = propertyContainer.property<double>();
	x *= 2;

	// Check that propertyContainer.property<double>() didn't change
	checkEqual(propertyContainer.property<double>(), doubleValue);
}

struct ATest
{
	int f()
	{
		return 5;
	}
};

TestCase(PropertyContainerCallPropertyMemberFunctionTest)
{
	int returnValue = 5;
	PropertyContainer<ATest> propertyContainer;

	checkEqual(propertyContainer.property<ATest>().f(), returnValue);
}

TestCase(MassInPropertyContainerTest)
{
	double massValue = 5;
	PropertyContainer<Mass> propertyContainer;

	check( !( propertyContainer.assigned<Mass>() ) );

	propertyContainer.property<Mass>().set(massValue);

	check(( propertyContainer.assigned<Mass>() ));

	checkEqual(propertyContainer.property<Mass>().get(), massValue);
}

// #include <ValuedPropertyContainer.hpp>
// #include <PropertyDefinitions.hpp>

// using namespace PropertyDefinitions;

// TestCase(PropertyContainerTest)
// {
// 	PropertyContainer<Mass, Volume> raw;
// 	raw.insertProperty<Mass>();

// 	SharedPointer< set<string> > nameSet = raw.getPropertyNames();
// 	set<string>::iterator it = nameSet->begin();

// 	checkEqual( *it, Mass::name );

// 	PropertyContainer<Mass, Volume> raw2(raw);
// 	raw2.insertProperty<Volume>();

// 	nameSet = raw.getPropertyNames();
// 	it = nameSet->find( Volume::name );

// 	checkEqual(*it, Volume::name);

// 	// Testing set and get input and output methods
// 	Property<double> newProperty;
// 	newProperty.setInputMethod(defaultInputMethod<double>);
// 	newProperty.setOutputMethod(defaultOutputMethod<double>);

// 	raw2.insertProperty<Property<double>>();

// 	// Test if raw2.getInputMethod( newProperty.getName() ) == defaultInputMethod<double>
// 	// and raw2.getOutputMethod(newProperty.getName()) == defaultOutputMethod<double>
// }

// struct Color : Property<string>
// {
// 	Color()
// 	{}

// 	static const std::string name;
// };
// const std::string Color::name = "Color";

// struct Integer : Property<int>
// {
// 	Integer()
// 	{}

// 	static const std::string name;
// };
// const std::string Integer::name = "Integer";

// TestCase(ValuedPropertyContainerTest)
// {
// 	double massValue = 80.5;
// 	double volumeValue = 10.0;
// 	string colorValue = "blue";
// 	int intValue = 5;

// 	std::vector< Any > valueList;
// 	set<string> nameList;

// 	ValuedPropertyContainer<Mass, Color, Integer, Volume> valuedPropertyContainer;

// 	valuedPropertyContainer.setProperty(Mass(), massValue);
// 	valuedPropertyContainer.setProperty(Color(), colorValue);
// 	valuedPropertyContainer.setProperty(Integer(), intValue);
// 	valuedPropertyContainer.setProperty(Volume(), volumeValue);

// 	checkEqual(valuedPropertyContainer.getValue(Mass()), massValue);
// 	checkEqual(valuedPropertyContainer.getValue(Color()), colorValue);
// 	checkEqual(valuedPropertyContainer.getValue(Integer()), intValue);
// 	checkEqual(valuedPropertyContainer.getValue(Volume()), volumeValue);

// 	check(valuedPropertyContainer.checkSetted(Mass()));
// 	check(valuedPropertyContainer.checkSetted(Color::name));
// 	check(!valuedPropertyContainer.checkSetted("length"));	// Checks that "length" was not set

// 	PropertyContainer<Mass> raw;
// 	raw.insertProperty<Mass>();

// 	ValuedPropertyContainer<Mass> propertyContainer2(raw);
// 	SharedPointer<set<string>> nameSet = propertyContainer2.getPropertyNames();
// 	set<string>::iterator it = nameSet->find(Mass::name);
	
// 	checkEqual( *it, Mass::name );

// }
