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
		EmissiveMaterial( const std::string & p_name, const Vec3f & p_diffuse, const Vec3f & p_specular, const float p_maxDepth )
			: BaseMaterial( p_name ), _diffuse( p_diffuse ), _specular( p_specular, SPECULAR_LEN ), _maxDepth(p_maxDepth)
		{
		}

		virtual ~EmissiveMaterial() = default;

		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			float	  distance = 0.f;
			HitRecord hr;
			Ray		  r( p_hitRecord._point, p_ray.getDirection() );
			if ( p_hitRecord._object->intersect( r, 0.001f, 10000.f, hr ) )
			{
				distance = glm::distance( p_hitRecord._point, hr._point );
			}
			float effectiveDepth = ( ( distance*distance ) / _maxDepth );
			float area			 = 4.f * PIf * ( _maxDepth / 2.f ) * ( _maxDepth / 2.f );
			return ( _specular.getKs() / area ) * effectiveDepth
				   + ( 1.f - effectiveDepth ) * ( _diffuse.getKd() / area );
		}

		inline const Vec3f & getFlatColor() const override { return _diffuse.getKd(); }

	  protected:
		PhongBRDF	_specular;
		LambertBRDF _diffuse;
		float		_maxDepth;
	};

} // namespace RT_ISICG
