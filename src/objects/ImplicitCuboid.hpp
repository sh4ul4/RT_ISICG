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
		virtual Vec3f _evaluateNormal( const Vec3f & p_point ) const
		{
			Vec3f v	  = abs( p_point - _center ) - _dims;
			float max = vmax( v );
			Vec3f res;
			if ( v.x == max )
				res = Vec3f( 1.f, 0.f, 0.f );
			else if ( v.y == max )
				res = Vec3f( 0.f, 1.f, 0.f );
			else
				res = Vec3f( 0.f, 0.f, 1.f );
			if ( glm::dot( res, p_point ) > 0.f ) return -res;
			return glm::normalize(res);
		}
	};
} // namespace RT_ISICG
