#pragma once

#include "base_material.hpp"

namespace RT_ISICG
{
	class TransparentMaterial : public BaseMaterial
	{
	  public:
		TransparentMaterial( const std::string & p_name, const float p_ior = 1.3f ) : BaseMaterial( p_name ), _ior( p_ior ) {}

		virtual ~TransparentMaterial() = default;

		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			return BLACK;
		}

		inline const Vec3f & getFlatColor() const override { return BLACK; }

		bool isTransparent() const override { return true; }

		float getIOR() const override { return _ior; }

	  protected:
		float _ior = 1.3f;
	};

} // namespace RT_ISICG
