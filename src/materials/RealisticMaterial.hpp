#pragma once

#include "base_material.hpp"
#include "brdfs/CookTorrance.hpp"
#include "brdfs/OrenNayarBRDF.hpp"

namespace RT_ISICG
{
	class RealisticMaterial : public BaseMaterial
	{
		Vec3f polar( const Vec3f & cartesian ) const
		{
			const float radius
				= sqrt( cartesian.x * cartesian.x + cartesian.y * cartesian.y + cartesian.z * cartesian.z );
			const float theta = glm::atan( cartesian.y, cartesian.x );
			// const float theta = glm::acos( glm::dot( Vec2f( 1.f, 0.f ), Vec2f( cartesian.x, cartesian.y ) ) );
			const float phi = glm::acos( cartesian.z );
			return Vec3f( phi, theta, radius );
		}
	  public:
		RealisticMaterial( const std::string & p_name, const Vec3f & p_diffuse, const Vec3f & p_specular )
			: BaseMaterial( p_name ), _diffuse( p_diffuse, 0.6f ),
			  _specular( p_specular, 0.3f, Vec3f( ( 1.f, 0.85f, 0.57f ) ) ), _metalness(1.f)
		{
		}

		virtual ~RealisticMaterial() = default;

		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			const Vec3f wi = polar( glm::normalize( p_lightSample._direction ) );
			const Vec3f wo = polar( glm::normalize( -p_ray.getDirection() ) );
			return ( 1 - _metalness ) * _diffuse.evaluate( wi, wo )
				   + _metalness * _specular.evaluate( wi, wo, p_hitRecord._normal );
		}

		inline const Vec3f & getFlatColor() const override { return _diffuse.getKd(); }

	  protected:
		CookTorranceBRDF _specular;
		OrenNayarBRDF	 _diffuse;
		float			 _metalness = 0.f;
	};

} // namespace RT_ISICG
