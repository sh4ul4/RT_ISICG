#pragma once
#include "objects/implicit_surface.hpp"

namespace RT_ISICG
{
	class ImplicitSphere : public ImplicitSurface
	{
	  public:
		ImplicitSphere()		  = delete;
		virtual ~ImplicitSphere() = default;

		ImplicitSphere( const std::string & p_name, const Vec3f & p_center, const float p_radius )
			: ImplicitSurface( p_name ), _center( p_center ), _radius( p_radius )
		{
		}

	  private:
		Vec3f		  _center;
		float		  _radius;
		virtual float _sdf( const Vec3f & p_point ) const { return glm::distance( p_point, _center ) - _radius; }
	};
} // namespace RT_ISICG