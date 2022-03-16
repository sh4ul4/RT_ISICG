#pragma once

#include "base_material.hpp"
#include "brdfs/OrenNayarBRDF.hpp"

namespace RT_ISICG
{
	class MatteMaterial : public BaseMaterial
	{
		Vec3f polar( const Vec3f & cartesian ) const
		{
			const float radius
				= sqrt( cartesian.x * cartesian.x + cartesian.y * cartesian.y + cartesian.z * cartesian.z );
			const float theta = glm::atan( cartesian.y, cartesian.x );
			//const float theta = glm::acos( glm::dot( Vec2f( 1.f, 0.f ), Vec2f( cartesian.x, cartesian.y ) ) );
			const float phi	  = glm::acos( cartesian.z );
			return Vec3f( phi, theta, radius );
		}
	  public:
		MatteMaterial( const std::string & p_name, const Vec3f & p_diffuse, const float p_sigma )
			: BaseMaterial( p_name ), _brdf( p_diffuse, p_sigma )
		{
		}

		virtual ~MatteMaterial() = default;

		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			const Vec3f wi = polar( glm::normalize( p_lightSample._direction ) );
			const Vec3f wo = polar( glm::normalize( -p_ray.getDirection() ) );
			return _brdf.evaluate(wi, wo);
		}

		inline const Vec3f & getFlatColor() const override { return _brdf.getKd(); }

	  protected:
		OrenNayarBRDF _brdf;
	};

} // namespace RT_ISICG
