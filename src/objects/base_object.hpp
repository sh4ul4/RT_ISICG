#ifndef __RT_ISICG_BASE_OBJECT__
#define __RT_ISICG_BASE_OBJECT__

#include "defines.hpp"
#include "hit_record.hpp"
#include "materials/base_material.hpp"
#include "ray.hpp"
#include <vector>

namespace RT_ISICG
{
	class BaseObject
	{
	  public:
		BaseObject( const std::string & p_name ) : _name( p_name ) {}
		virtual ~BaseObject() = default;

		virtual const std::string & getName() const final { return _name; }

		virtual inline BaseMaterial * getMaterial() const final { return _material; }
		virtual inline void			  setMaterial( BaseMaterial * p_material ) final { _material = p_material; }

		// Check for nearest intersection: if found fill p_hitRecord.
		virtual bool intersect( const Ray & p_ray,
								const float p_tMin,
								const float p_tMax,
								HitRecord & p_hitRecord ) const = 0;

	  protected:
		const std::string _name;
		BaseMaterial *	  _material = nullptr;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_BASE_OBJECT__
