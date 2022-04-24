#pragma once

#include "utils/random.hpp"
#include "scene.hpp"
#include "integrators/PhotonCastIntegrator.hpp"

namespace RT_ISICG
{
	class PhotonCaster
	{
		// number of potential photons per light-source
		const uint8_t nPhotonsPerLight = 10000000;

	  public:
		PhotonCastIntegrator _integrator;
		PhotonKd3			 kd3;
		void				 cast( const Scene & p_scene )
		{
			for ( BaseLight * bl : p_scene.getLights() )
			{
				std::vector<PhotonKd3::Photon> lightPhotons;
				for ( uint8_t i = 0; i < nPhotonsPerLight; i++ )
				{
					// trace ray
					Ray ray( bl->getPos(),
							 glm::normalize( Vec3f( ( randomFloat() * 2.f ) - 1.f,
													( randomFloat() * 2.f ) - 1.f,
													( randomFloat() * 2.f ) - 1.f ) ) );
					// give photon vector to integrator
					_integrator.Li( p_scene, ray, 0.001f, 10000.f, lightPhotons, bl );
				}
				// true number of photons per light-source
				const float nbPhotons = lightPhotons.size();
				for ( PhotonKd3::Photon & p : lightPhotons )
				{
					// define power
					Vec3f pow = p.pow / nbPhotons; // / ( glm::distance( bl->getPos(), p.pos )/2.f );
					if ( glm::length( pow ) < glm::epsilon<float>() ) continue;
					// add calculated photons to kd-tree
					kd3.photons.emplace_back( p.pos, pow );
				}
			}
			// build kd-tree
			kd3.build();
		}
	};
} // namespace RT_ISICG