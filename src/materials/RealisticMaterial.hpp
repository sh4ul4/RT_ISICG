#pragma once

#include "base_material.hpp"
#include "brdfs/CookTorrance.hpp"
#include "brdfs/OrenNayarBRDF.hpp"

namespace RT_ISICG
{
	class RealisticMaterial : public BaseMaterial
	{
	  public:
		RealisticMaterial( const std::string & p_name, const Vec3f & p_diffuse, const Vec3f & p_specular, const float p_metalness )
			: BaseMaterial( p_name ), _diffuse( p_diffuse, 0.6f ),
			  _specular( p_specular, 0.3f, Vec3f( ( 1.f, 0.85f, 0.57f ) ) ), _metalness( p_metalness )
		{
		}

		virtual ~RealisticMaterial() = default;

		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			const Vec3f wo = glm::normalize( -p_lightSample._direction );
			const Vec3f wi = glm::normalize( -p_ray.getDirection() );
			//const Vec3f wo = -p_lightSample._direction;
			//const Vec3f wi = -p_ray.getDirection();
			return ( 1 - _metalness ) * _diffuse.evaluate( wi, wo, p_hitRecord._normal )
				   + _metalness * _specular.evaluate( wi, wo, p_hitRecord._normal );
		}

		inline const Vec3f & getFlatColor() const override { return _diffuse.getKd(); }

	  protected:
		CookTorranceBRDF _specular;
		OrenNayarBRDF	 _diffuse;
		float			 _metalness = 0.f;
	};

} // namespace RT_ISICG
