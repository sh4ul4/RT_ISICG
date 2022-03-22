#pragma once

#include "base_material.hpp"
#include "brdfs/OrenNayarBRDF.hpp"

namespace RT_ISICG
{
	class MatteMaterial : public BaseMaterial
	{
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
			const Vec3f wo = glm::normalize( -p_lightSample._direction );
			const Vec3f wi = glm::normalize( -p_ray.getDirection() );
			return _brdf.evaluate( wi, wo, p_hitRecord._normal );
		}

		inline const Vec3f & getFlatColor() const override { return _brdf.getKd(); }

	  protected:
		OrenNayarBRDF _brdf;
	};

} // namespace RT_ISICG
