#pragma once

#include "base_material.hpp"
#include "brdfs/PhongBRDF.hpp"
#include "brdfs/lambert_brdf.hpp"

#define SPECULAR 0.f
#define SPECULAR_LEN 8.f

namespace RT_ISICG
{
	class EmissiveMaterial : public BaseMaterial
	{
	  public:
		EmissiveMaterial( const std::string & p_name, const Vec3f & p_diffuse, const Vec3f & p_specular )
			: BaseMaterial( p_name ), _diffuse( p_diffuse ), _specular( p_specular, SPECULAR_LEN )
		{
		}

		virtual ~EmissiveMaterial() = default;

		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			return _diffuse.evaluate();
		}

		inline const Vec3f & getFlatColor() const override { return _diffuse.getKd(); }

	  protected:
		PhongBRDF	_specular;
		LambertBRDF _diffuse;
	};

} // namespace RT_ISICG
