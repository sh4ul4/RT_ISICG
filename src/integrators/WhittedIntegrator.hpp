#pragma once

#include "base_integrator.hpp"
#include "lights/PointLight.hpp"

namespace RT_ISICG
{
	class WhittedIntegrator : public BaseIntegrator
	{
	  public:
		WhittedIntegrator() : BaseIntegrator() {}
		virtual ~WhittedIntegrator() = default;

		const IntegratorType getType() const override { return IntegratorType::DIRECT_LIGHTING; }

		Vec3f Li( const Scene & p_scene,
				  const Ray &	p_ray,
				  const float	p_tMin,
				  const float	p_tMax,
				  const float	p_nbLightSamples = 1 ) const;

	  private:
		Vec3f LiRec( const float   depth,
					 const bool inside,
					 const float   refractIdx,
					 const Scene & p_scene,
					 const Ray &   p_ray,
					 const float   p_tMin,
					 const float   p_tMax,
					 const float   p_nbLightSamples = 1 ) const;

		Vec3f _directLighting( const Ray &		   ray,
							   const LightSample & ls,
							   const HitRecord &   hitRecord,
							   const float		   cosTheta ) const;

		size_t _nbBounces = 5;

	  private:
		float fresnelEquation( const Vec3f & I, const Vec3f & N, const float & ior ) const
		{
			const float tmp	 = glm::dot( glm::normalize( I ), glm::normalize( N ) );
			float		cosi = tmp < -1.f ? -1.f : tmp > 1.f ? 1.f : tmp; // clamp
			float		etai = 1.f, etat = ior;
			if ( cosi > 0.f ) { std::swap( etai, etat ); }
			// Compute sini using Snell's law
			float sint = etai / etat * sqrtf( std::max( 0.f, 1.f - cosi * cosi ) );
			// Total internal reflection
			if ( sint >= 1.f ) { return 1.f; }
			else
			{
				float cost = sqrtf( std::max( 0.f, 1.f - sint * sint ) );
				cosi	   = fabsf( cosi );
				float Rs   = ( ( etat * cosi ) - ( etai * cost ) ) / ( ( etat * cosi ) + ( etai * cost ) );
				float Rp   = ( ( etai * cosi ) - ( etat * cost ) ) / ( ( etai * cosi ) + ( etat * cost ) );
				return ( Rs * Rs + Rp * Rp ) / 2.f;
			}
		}
		Vec3f refract( const Vec3f & I, const Vec3f & N, const float & ior ) const
		{
			const float tmp	 = glm::dot( glm::normalize( I ), glm::normalize( N ) );
			float		cosi = tmp < -1.f ? -1.f : tmp > 1.f ? 1.f : tmp; // clamp
			float etai = 1.f, etat = ior;
			Vec3f n = N;
			if ( cosi < 0.f ) { cosi = -cosi; }
			else
			{
				std::swap( etai, etat );
				n = -N;
			}
			float eta = etai / etat;
			float k	  = 1.f - eta * eta * ( 1.f - cosi * cosi );
			return k < 0.f ? Vec3f(0.f) : eta * I + ( eta * cosi - sqrtf( k ) ) * n;
		}
		Vec3f reflect( const Vec3f & I, const Vec3f & N ) const { return I - 2 * glm::dot( I, N ) * N; }
	};
} // namespace RT_ISICG