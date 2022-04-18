#pragma once
#include "objects/implicit_surface.hpp"

namespace RT_ISICG
{
	class ImplicitInterpolation : public ImplicitSurface
	{
	  public:
		ImplicitInterpolation() = delete;
		virtual ~ImplicitInterpolation()
		{
			for ( auto object : _objects )
				delete object;
		}

		ImplicitInterpolation( const std::string & p_name, const std::vector<ImplicitSurface *> & p_objects, const float p_t)
			: ImplicitSurface( p_name ), _objects( p_objects ), _t(p_t)
		{
		}

	  private:
		float						   _t;
		std::vector<ImplicitSurface *> _objects;
		float						   lerpFloat( const std::vector<float>& vec ) const
		{
			if ( vec.size() == 2 ) return _t * vec[ 0 ] + ( 1 - _t ) * vec[ 1 ];
			std::vector<float> newvec;
			for (size_t i = 0; i < vec.size() - 1; i++) {
				newvec.emplace_back( _t * vec[ i ] + ( 1 - _t ) * vec[ i + 1 ] );
			}
			return lerpFloat( newvec );
		}

		virtual float _sdf( const Vec3f & p_point ) const
		{
			std::vector<float> vec;
			for ( auto object : _objects )
				vec.emplace_back( object->_sdf( p_point ) );
			return lerpFloat( vec );
		}

		Vec3f lerpVec( const std::vector<Vec3f> & vec ) const
		{
			if ( vec.size() == 2 ) return _t * vec[ 0 ] + ( 1 - _t ) * vec[ 1 ];
			std::vector<Vec3f> newvec;
			for ( size_t i = 0; i < vec.size() - 1; i++ )
			{
				newvec.emplace_back( _t * vec[ i ] + ( 1 - _t ) * vec[ i + 1 ] );
			}
			return lerpVec( newvec );
		}

		virtual Vec3f _evaluateNormal( const Vec3f & p_point ) const
		{
			std::vector<Vec3f> vec;
			for ( auto object : _objects )
			{
				vec.emplace_back( object->_evaluateNormal( p_point ) );
			}
			return lerpVec( vec );
		}
	};
} // namespace RT_ISICG
