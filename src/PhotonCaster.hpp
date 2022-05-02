#pragma once

#include "utils/random.hpp"
#include "scene.hpp"
#include "integrators/PhotonCastIntegrator.hpp"
#include "utils/console_progress_bar.hpp"

#define CONSOLE_BAR 1

namespace RT_ISICG
{
	class PhotonCaster
	{
		// number of potential photons per light-source
		const int nPhotonsPerLight = 100000; // recommended values lay between 10000 and 1000000

	  public:
		PhotonCastIntegrator _integrator;
		PhotonKd3			 kd3;
		void				 cast( const Scene & p_scene )
		{
#if CONSOLE_BAR
			ConsoleProgressBar progressBar;
			progressBar.start( p_scene.getLights().size() * nPhotonsPerLight, 50 );
#endif
			for ( BaseLight * bl : p_scene.getLights() )
			{
				std::vector<PhotonKd3::Photon> lightPhotons;
#pragma omp parallel for schedule( dynamic )
				for ( int i = 0; i < nPhotonsPerLight; i++ )
				{
					// trace ray
					Ray ray( bl->getPos(),
							 glm::normalize( Vec3f( ( randomFloat() * 2.f ) - 1.f,
													( randomFloat() * 2.f ) - 1.f,
													( randomFloat() * 2.f ) - 1.f ) ) );
					// give photon vector to integrator
					_integrator.Li( p_scene, ray, 0.001f, 10000.f, lightPhotons, bl );
#if CONSOLE_BAR
					progressBar.next();
#endif
				}
				// true number of photons per light-source
				const float nbPhotons = lightPhotons.size();
				for ( PhotonKd3::Photon & p : lightPhotons )
				{
					// define power
					Vec3f pow = p.pow / nbPhotons;
					if ( glm::length( pow ) < glm::epsilon<float>() ) continue;
					// add calculated photons to kd-tree
					kd3.photons.emplace_back( p.pos, pow );
				}
			}
#if CONSOLE_BAR
			progressBar.stop();
#endif
			std::cout << nPhotonsPerLight * p_scene.getLights().size() << " photons casted" << std::endl;
			// build kd-tree
			kd3.build();
		}
	};
} // namespace RT_ISICG