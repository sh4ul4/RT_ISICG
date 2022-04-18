#ifndef __RT_ISICG_PERSPECTIVE_CAMERA__
#define __RT_ISICG_PERSPECTIVE_CAMERA__

#include "base_camera.hpp"
#include "utils/random.hpp"

namespace RT_ISICG
{
	class PerspectiveCamera : public BaseCamera
	{
	  public:
		PerspectiveCamera( const float p_aspectRatio );

		PerspectiveCamera( const Vec3f & p_position,
						   const Vec3f & p_lookAt,
						   const Vec3f & p_up,
						   const float	 p_fovy,
						   const float	 p_aspectRatio );

		~PerspectiveCamera() = default;

		inline Vec2f getRandomApperturePoint() const {
			// circular apperture
			float  angle  =  randomFloat() * 2.f * PIf;
			float radius = sqrt( randomFloat() );
			return Vec2f( cos( angle ), sin( angle ) ) * radius * _apertureRadius;
		}

		inline Ray generateRay( const float p_sx, const float p_sy ) const override
		{
			Vec2f ap = getRandomApperturePoint();
			Vec3f viewportPos = _viewportTopLeftCorner + p_sx * _viewportU - p_sy * _viewportV;
			Vec3f focalPointPos = viewportPos + ap.x * _u - ap.y * _v - _focalDistance * _w;
			Vec3f direction = glm::normalize( focalPointPos - _position );
			return Ray( viewportPos, direction );
		}

	  private:
		void _updateViewport();

	  private:
		float _fovy			 = 60.f;
		float _focalDistance = 0.0001f;
		float _aspectRatio	 = 1.f;
		float _apertureRadius = 0.0001f;

		// Local coordinates system
		Vec3f _u = Vec3f( 1.f, 0.f, 0.f );
		Vec3f _v = Vec3f( 0.f, 1.f, 0.f );
		Vec3f _w = Vec3f( 0.f, 0.f, -1.f );

		// Viewport data
		Vec3f _viewportTopLeftCorner = VEC3F_ZERO; // Top left corner position
		Vec3f _viewportU			 = VEC3F_ZERO; // Horizontal vector
		Vec3f _viewportV			 = VEC3F_ZERO; // Vertical vector
	};
} // namespace RT_ISICG

#endif // __RT_ISICG_PERSPECTIVE_CAMERA__
