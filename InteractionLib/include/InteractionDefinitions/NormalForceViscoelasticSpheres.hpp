#ifndef NORMAL_FORCE_HERTZ_HPP
#define NORMAL_FORCE_HERTZ_HPP

// EntityLib
#include <SphericalParticle.hpp>

// UtilsLib
#include <Builder.hpp>
#include <NamedType.hpp>
#include <mp/bool_constant.hpp>

// JSONLib
#include <json.hpp>

namespace psin {

// ------------------ FORCE CALCULATION ------------------
//		particle is the reference
//		normalForce is the normal force applied BY neighbor TO particle
//		tangentialForce is the tangential force applied BY neighbor TO particle

//		Calculates normal forces between two spherical particles according to equation (2.14) (see reference)
struct NormalForceViscoelasticSpheres
{
	template<typename P1, typename P2>
	struct check : mp::bool_constant<
		has_property<P1, Radius>::value
		and has_property<P1, ElasticModulus>::value
		and has_property<P1, DissipativeConstant>::value
		and has_property<P1, PoissonRatio>::value
		and is_spherical<P1>::value

		and has_property<P2, Radius>::value
		and has_property<P2, ElasticModulus>::value
		and has_property<P2, DissipativeConstant>::value
		and has_property<P2, PoissonRatio>::value
		and is_spherical<P2>::value
		>
	{};

	template<typename...Ts, typename...Us, typename Time>
	static Vector3D calculate(SphericalParticle<Ts...> & particle, SphericalParticle<Us...> & neighbor, const Time &);
};

template<typename I>
void initializeInteraction(const json & j);

template<>
void initializeInteraction<NormalForceViscoelasticSpheres>(const json & j);

template<typename I>
void finalizeInteraction();

template<>
void finalizeInteraction<NormalForceViscoelasticSpheres>();

} // psin

#include <InteractionDefinitions/NormalForceViscoelasticSpheres.tpp>

#endif
