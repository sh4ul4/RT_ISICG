#ifndef __RT_ISICG_BASE_LIGHT__
#define __RT_ISICG_BASE_LIGHT__

#include "light_sample.hpp"

namespace RT_ISICG
{
	class BaseLight
	{
	  public:
		BaseLight( const Vec3f & p_color, const float p_power = 1.f ) : _color( p_color ), _power(p_power) {}
		virtual ~BaseLight() = default;

		inline const Vec3f & getFlatColor() const { return _color; }

		virtual LightSample sample( const Vec3f & p_point ) const = 0;

		bool isSurface() const { return _isSurface;}

	  protected:
		// TODO: give a name, like objects and materials
		Vec3f _color = WHITE;
		float _power;
		bool  _isSurface = false;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_BASE_LIGHT__
