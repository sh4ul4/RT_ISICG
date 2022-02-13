#ifndef __RT_ISICG_BASE_GEOMETRY__
#define __RT_ISICG_BASE_GEOMETRY__

#include "hit_record.hpp"
#include "ray.hpp"

namespace RT_ISICG
{
	class BaseGeometry
	{
	  public:
		BaseGeometry()			= default;
		virtual ~BaseGeometry() = default;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_BASE_GEOMETRY__
