#pragma once
#include "objects/implicit_surface.hpp"

namespace RT_ISICG
{
	class ImplicitCuboid : public ImplicitSurface
	{
	  public:
		ImplicitCuboid()		  = delete;
		virtual ~ImplicitCuboid() = default;

		ImplicitCuboid( const std::string & p_name, const Vec3f & p_center, const Vec3f & p_dimensions )
			: ImplicitSurface( p_name ), _center( p_center ), _dims( p_dimensions )
		{
		}

	  private:
		Vec3f _center;
		Vec3f _dims;

		float		  vmax( Vec3f v ) const { return std::max( std::max( v.x, v.y ), v.z ); }
		virtual float _sdf( const Vec3f & p_point ) const { return vmax( abs( p_point - _center ) - _dims ); }
	};
} // namespace RT_ISICG
