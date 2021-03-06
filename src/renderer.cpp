#include "renderer.hpp"
#include "integrators/ray_cast_integrator.hpp"
#include "integrators/DirectLightingIntegrator.hpp"
#include "integrators/WhittedIntegrator.hpp"
#include "utils/console_progress_bar.hpp"
#include "utils/random.hpp"

#include "PhotonCaster.hpp"
#define PHOTONCAST 1

//#define TP1_EX1
//#define TP1_EX2 // without antialiasing
#define TP1_EX3 // with antialiasing

#ifdef TP1_EX3
#define __ALEA__ 1
#endif

namespace RT_ISICG
{
	Renderer::Renderer() { _integrator = new RayCastIntegrator(); }

	void Renderer::setIntegrator( const IntegratorType p_integratorType )
	{
		if ( _integrator != nullptr ) { delete _integrator; }

		switch ( p_integratorType )
		{
		case IntegratorType::RAY_CAST: _integrator = new RayCastIntegrator(); break;
		case IntegratorType::DIRECT_LIGHTING: _integrator = new DirectLightingIntegrator(); break;
		case IntegratorType::WHITTED_LIGHTING: _integrator = new WhittedIntegrator(); break;
		default:
		{
			_integrator = new RayCastIntegrator();
			break;
		}
		}
	}

	void Renderer::setBackgroundColor( const Vec3f & p_color )
	{
		if ( _integrator == nullptr ) { std::cout << "[Renderer::setBackgroundColor] Integrator is null" << std::endl; }
		else
		{
			_integrator->setBackgroundColor( p_color );
		}
	}

	float Renderer::renderImage( const Scene & p_scene, const BaseCamera * p_camera, Texture & p_texture )
	{
#if PHOTONCAST
		PhotonCaster pc;
		pc.cast( p_scene );
		if ( _integrator->getType() == IntegratorType::WHITTED_LIGHTING )
			dynamic_cast<WhittedIntegrator *>( _integrator )->pc = &pc;
		else std::cout << "not whitted lighting" << std::endl;
#endif
#ifdef TP1_EX3
		srand( time( 0 ) );
#endif

		const int width	 = p_texture.getWidth();
		const int height = p_texture.getHeight();

		Chrono			   chrono;
		ConsoleProgressBar progressBar;

		progressBar.start( height, 50 );
		chrono.start();
#pragma omp parallel for schedule(dynamic)
		for ( int j = 0; j < height; j++ )
		{
			for ( int i = 0; i < width; i++ )
			{
#ifdef TP1_EX3
				Vec3f color( 0.f );
				for ( int raycntr = 0; raycntr < _nbPixelSamples; raycntr++ )
				{
#if __ALEA__
					const float p_sx  = ( i + randomFloat() ) / ( width - 1 );
					const float p_sy  = ( j + randomFloat() ) / ( height - 1 );
					const Ray	ray	  = p_camera->generateRay( p_sx, p_sy );
#else
					int			root  = sqrt( _nbPixelSamples );
					int			x = raycntr / root;
					int			y = raycntr % root;
					const float p_sx  = ( i + (float)x / _nbPixelSamples ) / ( width - 1 );
					const float p_sy  = ( j + (float)y / _nbPixelSamples ) / ( height - 1 );
					const Ray	ray	  = p_camera->generateRay( p_sx, p_sy );
#endif
#else
				const Ray ray = p_camera->generateRay( ( i + 0.5f ) / ( width - 1 ), ( j + 0.5f ) / ( height - 1 ) );
#endif
#ifdef TP1_EX2
					Vec3f color = _integrator->Li( p_scene, ray, 0.f, 10000.f, _nbLightSamples );
					p_texture.setPixel( i, j, glm::clamp(color,Vec3f(0.f),Vec3f(1.f) ));
#endif
#ifdef TP1_EX1
					p_texture.setPixel( i, j, ( ray.getDirection() + 1.f ) * 0.5f );
#endif
#ifdef TP1_EX3
					Vec3f tmpcolor = _integrator->Li( p_scene, ray, 0.f, 10000.f, _nbLightSamples );
					color += tmpcolor;
				}
				color /= _nbPixelSamples;
				p_texture.setPixel( i, j, glm::clamp(color,Vec3f(0.f),Vec3f(1.f)) );
#endif
			}
			progressBar.next();
		}

		chrono.stop();
		progressBar.stop();

		return chrono.elapsedTime();
	}
} // namespace RT_ISICG
