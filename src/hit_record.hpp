#ifndef __RT_ISICG_HIT_RECORD__
#define __RT_ISICG_HIT_RECORD__

#include "defines.hpp"

namespace RT_ISICG
{
	class BaseObject;

	struct HitRecord
	{
		HitRecord() = default;
		HitRecord( const Vec3f & p_point, const Vec3f & p_normal, const float p_distance, const BaseObject * p_object )
			: _point( p_point ), _normal( p_normal ), _distance( p_distance ), _object( p_object )
		{
		}

		void faceNormal( const Vec3f p_direction )
		{
			_normal = glm::dot( p_direction, _normal ) < 0.f ? _normal : -_normal;
		}

		Vec3f			   _point	 = VEC3F_ZERO;
		Vec3f			   _normal	 = VEC3F_ZERO;
		float			   _distance = 0.f;
		const BaseObject * _object	 = nullptr;
	};
} // namespace RT_ISICG

#endif // __RT_ISICG_HIT_RECORD__
