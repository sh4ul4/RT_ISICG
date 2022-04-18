#pragma once
#include "objects/implicit_surface.hpp"

namespace RT_ISICG
{
	class ImplicitPlane : public ImplicitSurface
	{
	  public:
		ImplicitPlane()			 = delete;
		virtual ~ImplicitPlane() = default;

		ImplicitPlane( const std::string & p_name, const Vec3f & p_point, const Vec3f p_normal )
			: ImplicitSurface( p_name ), _point( p_point ), _normal( glm::normalize( p_normal ) )
		{
		}

	  private:
		Vec3f		  _point;
		Vec3f		  _normal;
		virtual float _sdf( const Vec3f & p_point ) const { return glm::dot( ( p_point - _point ), _normal ); }
		virtual Vec3f _evaluateNormal( const Vec3f & p_point ) const { return _normal; }
	};
} // namespace RT_ISICG