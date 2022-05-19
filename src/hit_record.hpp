#ifndef __RT_ISICG_HIT_RECORD__
#define __RT_ISICG_HIT_RECORD__

#include "defines.hpp"
#include <array>

namespace RT_ISICG
{
	class BaseObject;

	struct HitRecord
	{
		HitRecord() = default;

		void faceNormal( const Vec3f p_direction )
		{
			_normal = glm::dot( p_direction, _normal ) < 0.f ? _normal : -_normal;
		}

		Vec3f			   _point	 = VEC3F_ZERO;
		Vec3f			   _normal	 = VEC3F_ZERO;
		Vec2f			   _uv		 = VEC2F_ZERO;
		float			   _pixelConeRad = 0.f;
		float			   _pixelConeAlpha = 0.f;
		std::array<float, 6> _textureFootprint = {};
		float			   _distance = 0.f;
		const BaseObject * _object	 = nullptr;
	};
} // namespace RT_ISICG

#endif // __RT_ISICG_HIT_RECORD__
