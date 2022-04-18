#pragma once
#include "objects/implicit_surface.hpp"

namespace RT_ISICG
{
	class ImplicitIntersection : public ImplicitSurface
	{
	  public:
		ImplicitIntersection() = delete;
		virtual ~ImplicitIntersection()
		{
			for ( auto object : _objects )
				delete object;
		}

		ImplicitIntersection( const std::string & p_name, const std::vector<ImplicitSurface *> & p_objects )
			: ImplicitSurface( p_name ), _objects( p_objects )
		{
		}

	  private:
		std::vector<ImplicitSurface *> _objects;
		virtual float				   _sdf( const Vec3f & p_point ) const
		{
			float maximum = glm::epsilon<float>();
			for ( auto object : _objects )
			{
				const float sdf = object->_sdf( p_point );
				maximum			= std::max( sdf, maximum );
			}
			return maximum;
		}
		virtual Vec3f _evaluateNormal( const Vec3f & p_point ) const
		{
			float			  minimum		 = INFINITY;
			ImplicitSurface * closestSurface = _objects[ 0 ];
			for ( auto object : _objects )
			{
				const float sdf = object->_sdf( p_point );
				if ( sdf < minimum )
				{
					closestSurface = object;
					minimum		   = sdf;
				}
			}
			return closestSurface->_evaluateNormal( p_point );
		}
	};
} // namespace RT_ISICG