#include "perspective_camera.hpp"
#include <glm/gtx/string_cast.hpp>

namespace RT_ISICG
{
	PerspectiveCamera::PerspectiveCamera( const float p_aspectRatio ) : _aspectRatio( p_aspectRatio )
	{
		_updateViewport();
	}

	PerspectiveCamera::PerspectiveCamera( const Vec3f & p_position,
										  const Vec3f & p_lookAt,
										  const Vec3f & p_up,
										  const float	p_fovy,
										  const float	p_aspectRatio )
		: BaseCamera( p_position ), _fovy( p_fovy ), _aspectRatio( p_aspectRatio )
	{
		_v = glm::normalize( p_up );
		_w = -glm::normalize( p_lookAt );
		_u = glm::normalize( glm::cross( _v, _w ) );
		_updateViewport();
	}

	void PerspectiveCamera::_updateViewport()
	{
		float adj			   = _focalDistance;
		float hyp			   = adj / sin( glm::radians( _fovy ) / 2 );
		float opp			   = sqrt( hyp * hyp ) - sqrt( adj * adj );
		_viewportV			   = _v * ( opp );
		_viewportU			   = _u * ( ( opp ) * _aspectRatio );
		_viewportTopLeftCorner = _position - _w * _focalDistance - ( _viewportU / 2.f ) + ( _viewportV / 2.f );
	}

} // namespace RT_ISICG
