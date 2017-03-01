template<typename type>
void defaultSetter(const type & value, type & destination)
{
	destination = value;
}

template<typename type>
type defaultGetter(const type & value)
{
	return value;
}

template<typename type>
bool defaultInputMethod(ifstream & in, Any & value)
{
	type newValue;

	in >> newValue;
	value = newValue;

	return true;
}

template<typename type>
bool defaultOutputMethod(ofstream & out, Any & value)
{
	out << anyCast<type>(value);

	return true;
}



// Constructors

template<typename interfaceType, typename storedType>
Property<interfaceType, storedType>::Property()
	: name("Nameless")
{
	this->inputMethod = defaultInputMethod<interfaceType>;
	this->outputMethod = defaultOutputMethod<interfaceType>;
}

template<typename interfaceType, typename storedType>
Property<interfaceType, storedType>::Property(const string & name)
{
	this->setName(name);

	this->inputMethod = defaultInputMethod<interfaceType>;
	this->outputMethod = defaultOutputMethod<interfaceType>;
}

template<typename interfaceType, typename storedType>
Property<interfaceType, storedType>::Property(const string & name, void (*setterFunction)(const interfaceType &, storedType &), interfaceType (*getterFunction)(const storedType &))
{
	this->name = name;

	this->setter = setterFunction;
	this->getter = getterFunction;

	this->inputMethod = defaultInputMethod<interfaceType>;
	this->outputMethod = defaultOutputMethod<interfaceType>;
}


// Set and get name

template<typename interfaceType, typename storedType>
void Property<interfaceType, storedType>::setName(const string & name)
{
	if(!name.empty()) this->name = name;
	else this->name = "Nameless";
}

template<typename interfaceType, typename storedType>
string Property<interfaceType, storedType>::getName(void) const
{
	return this->name;
}


// Set setter and getter
template<typename interfaceType, typename storedType>
void Property<interfaceType, storedType>::setSetterFunction( void (*setterFunction)(const interfaceType & value, storedType & destination) )
{
	this->setter = setterFunction;
}

template<typename interfaceType, typename storedType>
void Property<interfaceType, storedType>::setGetterFunction( interfaceType (*getterFunction)(const storedType & value) )
{
	this->getter = getterFunction;
}

// Set inputMethod and outputMethod
template<typename interfaceType, typename storedType>
void Property<interfaceType, storedType>::setInputMethod( inputMethodType newInputMethod )
{
	this->inputMethod = newInputMethod;
}

template<typename interfaceType, typename storedType>
void Property<interfaceType, storedType>::setOutputMethod( outputMethodType newOutputMethod )
{
	this->outputMethod = newOutputMethod;
}


// Only one type

// Constructors
// If types are equal, we are allowed to use defaultSetter and defaultGetter (copy setter and getters)
template<typename type>
Property<type>::Property()
	: Property<type, type>()
{
	setter = defaultSetter;
	getter = defaultGetter;
}

// If types are equal, we are allowed to use defaultSetter and defaultGetter (copy setter and getters)
template<typename type>
Property<type>::Property(const string & name, void (*setterFunction)(const type &, type &), type (*getterFunction)(const type &))
	: Property<type, type>(name, setterFunction, getterFunction)
{
}