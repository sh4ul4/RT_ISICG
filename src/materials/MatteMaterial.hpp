#pragma once

#include "base_material.hpp"
#include "brdfs/OrenNayarBRDF.hpp"

namespace RT_ISICG
{
	class MatteMaterial : public BaseMaterial
	{
	  public:
		MatteMaterial( const std::string & p_name, const Vec3f & p_diffuse )
			: BaseMaterial( p_name ), _brdf( p_diffuse, 0.f )
		{
		}

		virtual ~MatteMaterial() = default;

		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			const Vec3f wi = glm::polar( glm::normalize( p_lightSample._direction ) );
			const Vec3f wo = glm::polar( glm::normalize( -p_ray.getDirection() ) );
			return _brdf.evaluate(wi, wo);
		}

		inline const Vec3f & getFlatColor() const override { return _brdf.getKd(); }

	  protected:
		OrenNayarBRDF _brdf;
	};

} // namespace RT_ISICG
