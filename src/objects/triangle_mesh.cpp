#include "triangle_mesh.hpp"

//#define AABB_CHECK
#define BVH_CHECK

namespace RT_ISICG
{
	bool MeshTriangle::intersect( const Ray & p_ray,
								  const float p_tMin,
								  const float p_tMax,
								  HitRecord & p_hitRecord ) const
	{
#ifdef AABB_CHECK
		if ( !_aabb.intersect( p_ray, p_tMin, p_tMax ) ) { return false; }
#endif
#ifdef BVH_CHECK
		const bool intersection = _bvh.intersect( p_ray, p_tMin, p_tMax, p_hitRecord );
		if ( intersection ) p_hitRecord._object = this;
		return intersection;
#endif
		float  tClosest = p_tMax;			 // Hit distance.
		size_t hitTri	= _triangles.size(); // Hit triangle id.
		float  u = 0.f, v = 0.f;
		for ( size_t i = 0; i < _triangles.size(); i++ )
		{
			float t, utmp, vtmp;
			if ( _triangles[ i ].intersect( p_ray, t, utmp, vtmp ) )
			{
				if ( t >= p_tMin && t <= tClosest )
				{
					tClosest = t;
					hitTri	 = i;
					u		 = utmp;
					v		 = vtmp;
				}
			}
		}
		if ( hitTri != _triangles.size() ) // Intersection found.
		{
			p_hitRecord._point	= p_ray.pointAtT( tClosest );
			p_hitRecord._normal = _triangles[ hitTri ].getInterpolatedFaceNormal( u, v );
			p_hitRecord.faceNormal( p_ray.getDirection() );
			p_hitRecord._distance = tClosest;
			p_hitRecord._object	  = this;
			p_hitRecord._uv		  = _triangles[ hitTri ].getInterpolatedTextureCoords( u, v );
			p_hitRecord._pixelConeRad += p_hitRecord._distance * glm::tan( p_hitRecord._pixelConeAlpha );
			_triangles[ hitTri ].getUvs(p_hitRecord._textureFootprint);
			return true;
		}
		return false;
	}

	bool MeshTriangle::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
#ifdef AABB_CHECK
		if ( !_aabb.intersect( p_ray, p_tMin, p_tMax ) ) { return false; }
#endif
#ifdef BVH_CHECK
		return _bvh.intersectAny( p_ray, p_tMin, p_tMax );
#endif
		for ( size_t i = 0; i < _triangles.size(); i++ )
		{
			float u, v;
			float t;
			if ( _triangles[ i ].intersect( p_ray, t, u, v ) )
			{
				if ( t >= p_tMin && t <= p_tMax ) return true; // No need to search for the nearest.
			}
		}
		return false;
	}
} // namespace RT_ISICG
