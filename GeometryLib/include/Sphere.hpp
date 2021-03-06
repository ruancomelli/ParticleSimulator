#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <stdexcept>

class Sphere
{
	public:
		Sphere();
		virtual ~Sphere();

		void setRadius( double radius );
		double getRadius( void );

		double getSurfaceArea( void );
		double getVolume( void );
		
	private:
		double radius;
};

typedef shared_ptr< Sphere > SpherePtr;

#endif
