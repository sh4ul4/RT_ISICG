#ifndef __RT_ISICG_BASE_MATERIAL__
#define __RT_ISICG_BASE_MATERIAL__

#include "hit_record.hpp"
#include "lights/light_sample.hpp"
#include "ray.hpp"

namespace RT_ISICG
{
	class BaseMaterial
	{
	  public:
		BaseMaterial() = delete;
		BaseMaterial( const std::string & p_name ) : _name( p_name ) {}
		virtual ~BaseMaterial() = default;

		virtual Vec3f shade( const Ray &		 p_ray,
							 const HitRecord &	 p_hitRecord,
							 const LightSample & p_lightSample ) const = 0;

		virtual inline const Vec3f & getFlatColor() const = 0;

		virtual const std::string & getName() const final { return _name; }

	  protected:
		std::string _name;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_BASE_MATERIAL__
