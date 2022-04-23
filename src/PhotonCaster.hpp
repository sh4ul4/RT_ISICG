#pragma once

#include "utils/random.hpp"
#include "scene.hpp"
#include "integrators/PhotonCastIntegrator.hpp"

namespace RT_ISICG
{
	class PhotonCaster
	{
	  public:
		PhotonCastIntegrator _integrator;
		PhotonKd3			 kd3;
		void				 cast( const Scene & p_scene )
		{
			int nPhotonsPerLight = 8000000;
			for ( BaseLight * bl : p_scene.getLights() )
			{
				for ( int i = 0; i < nPhotonsPerLight; i++ )
				{
					Ray							   ray( bl->getPos(),
								glm::normalize( Vec3f( ( randomFloat() * 2.f ) - 1.f,
													   ( randomFloat() * 2.f ) - 1.f,
													   ( randomFloat() * 2.f ) - 1.f ) ) );
					std::vector<PhotonKd3::Photon> photons;
					_integrator.Li( p_scene,
									ray,
									0.001f,
									10000.f,
									nPhotonsPerLight,
									photons,
									bl ); // give photon vector to integrator
					for ( PhotonKd3::Photon & p : photons )
					{
						Vec3f pow = p.pow;						// / ( glm::distance( bl->getPos(), p.pos )/2.f );
						kd3.photons.emplace_back( p.pos, pow ); // add calculated photons to kd-tree
					}
				}
			}
			// build kd-tree
			kd3.build();
		}
	};
} // namespace RT_ISICG