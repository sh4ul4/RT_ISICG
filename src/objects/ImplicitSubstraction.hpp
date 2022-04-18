#pragma once
#include "objects/implicit_surface.hpp"

namespace RT_ISICG
{
	class ImplicitSubstraction : public ImplicitSurface
	{
	  public:
		ImplicitSubstraction() = delete;
		virtual ~ImplicitSubstraction()
		{
			delete _objLeft;
			delete _objRight;
		}

		ImplicitSubstraction( const std::string & p_name, ImplicitSurface * p_objLeft, ImplicitSurface * p_objRight )
			: ImplicitSurface( p_name ), _objLeft( p_objLeft ), _objRight( p_objRight )
		{
		}

	  private:
		ImplicitSurface * _objLeft;
		ImplicitSurface * _objRight;
		virtual float	  _sdf( const Vec3f & p_point ) const
		{
			return std::max( _objLeft->_sdf( p_point ), -_objRight->_sdf( p_point ) );
		}
		virtual Vec3f _evaluateNormal( const Vec3f & p_point ) const
		{
			ImplicitSurface * closestSurface = _objLeft;
			if ( _objLeft->_sdf( p_point ) > -_objRight->_sdf( p_point ) ) closestSurface = _objRight;
			return closestSurface->_evaluateNormal( p_point );
		}
	};
} // namespace RT_ISICG