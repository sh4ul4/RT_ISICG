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
		_w = glm::normalize( _position - p_lookAt );
		_u = -glm::normalize( glm::cross( _w, p_up ) );
		_v = glm::normalize( glm::cross( _w, _u ) );
		_updateViewport();
	}

	void PerspectiveCamera::_updateViewport()
	{
		const float h	   = glm::tan( glm::radians( _fovy ) / 2.0f ) * 2.0f;
		const float w	   = h * _aspectRatio;
		const Vec3f center = _position - _w;
		_viewportTopLeftCorner = center - _u * ( w / 2.0f ) + _v * ( h / 2.0f );
		_viewportU			   = _u * w;
		_viewportV			   = _v * h;
	}

} // namespace RT_ISICG
