#pragma once

#include "base_material.hpp"

namespace RT_ISICG
{
	class TransparentMaterial : public BaseMaterial
	{
	  public:
		TransparentMaterial( const std::string & p_name,
							 const float		 p_ior			= 1.3f,
							 const float		 p_transparency = 100.f,
							 const Vec3f &		 p_color		= WHITE )
			: BaseMaterial( p_name ), _ior( p_ior ), _transparency( p_transparency ), _color( p_color )
		{
		}

		virtual ~TransparentMaterial() = default;

		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			return BLACK;
		}

		inline const Vec3f & getFlatColor() const override { return _color; }

		bool isTransparent() const override { return true; }

		float getIOR() const override { return _ior; }

		float getTransparency() const override { return _transparency; }

	  protected:
		float _ior			= 1.3f;
		float _transparency = 100.f;
		Vec3f _color		= WHITE;
	};

} // namespace RT_ISICG
