#ifndef __RT_ISICG_BASE_CAMERA__
#define __RT_ISICG_BASE_CAMERA__

#include "defines.hpp"
#include "ray.hpp"

namespace RT_ISICG
{
	class BaseCamera
	{
	  public:
		BaseCamera()		  = default;
		BaseCamera( const Vec3f & p_position ) : _position( p_position ) {}
		virtual ~BaseCamera() = default;

		const Vec3f & getPosition() const { return _position; }

		virtual inline Ray generateRay( const float p_sx, const float p_sy ) const = 0;

	  protected:
		Vec3f _position = VEC3F_ZERO;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_BASE_CAMERA__
