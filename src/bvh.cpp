#include "bvh.hpp"
#include "geometry/triangle_mesh_geometry.hpp"
#include "utils/chrono.hpp"
#include <algorithm>

namespace RT_ISICG
{
	void BVH::build( std::vector<TriangleMeshGeometry> * p_triangles, BaseObject * p_object )
	{
		std::cout << "Building BVH..." << std::endl;
		if ( p_triangles == nullptr || p_triangles->empty() )
		{
			throw std::exception( "BVH::build() error: no triangle provided" );
		}
		_triangles = p_triangles;

		Chrono chr;
		chr.start();

		/// TODO
		_object = p_object;
		_root	= new BVHNode();
		_buildRec( _root, 0, _triangles->size() - 1, 0 );
		///

		chr.stop();

		std::cout << "[DONE]: " << chr.elapsedTime() << "s" << std::endl;
	}

	bool BVH::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax, HitRecord & p_hitRecord ) const
	{
		/// TODO
		if ( _root != nullptr ) return _intersectRec( _root, p_ray, p_tMin, p_tMax, p_hitRecord );
		return false;
	}

	bool BVH::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		/// TODO
		if ( _root != nullptr ) return _intersectAnyRec( _root, p_ray, p_tMin, p_tMax );
		return false;
	}

	void BVH::_buildRec( BVHNode *			p_node,
						 const unsigned int p_firstTriangleId,
						 const unsigned int p_lastTriangleId,
						 const unsigned int p_depth )
	{
		/// TODO
		p_node->_firstTriangleId = p_firstTriangleId;
		p_node->_lastTriangleId	 = p_lastTriangleId;
		for ( size_t i = p_firstTriangleId; i <= p_lastTriangleId; i++ )
		{
			Vec3f a, b, c;
			( *_triangles )[ i ].getVertices( a, b, c );
			p_node->_aabb.extend( a );
			p_node->_aabb.extend( b );
			p_node->_aabb.extend( c );
		}
		// divide data
		size_t largestAxis	 = p_node->_aabb.largestAxis();
		Vec3f  centroid		 = p_node->_aabb.centroid();
		float  divisionCoord = ((largestAxis == 0) ? centroid.x : ((largestAxis == 1) ? centroid.y : centroid.z));
		size_t divisionId	 = p_firstTriangleId;
		std::partition( _triangles->begin() + p_firstTriangleId,
						_triangles->begin() + p_lastTriangleId,
						[ divisionCoord, largestAxis ]( TriangleMeshGeometry & tmg )
						{
							Vec3f a, b, c;
							tmg.getVertices( a, b, c );
							switch ( largestAxis )
							{
							case 0: return ( a.x < divisionCoord && b.x < divisionCoord && c.x < divisionCoord ); break;
							case 1: return ( a.y < divisionCoord && b.y < divisionCoord && c.y < divisionCoord ); break;
							default:
								return ( a.z < divisionCoord && b.z < divisionCoord && c.z < divisionCoord );
								break;
							}
						} );
		// build aabb
		for ( size_t i = p_firstTriangleId; i <= p_lastTriangleId; i++ )
		{
			Vec3f a, b, c;
			( *_triangles )[ i ].getVertices( a, b, c );
			switch ( largestAxis )
			{
			case 0:
				if ( a.x < divisionCoord && b.x < divisionCoord && c.x < divisionCoord ) divisionId = i;
				break;
			case 1:
				if ( a.y < divisionCoord && b.y < divisionCoord && c.y < divisionCoord ) divisionId = i;
				break;
			default:
				if ( a.z < divisionCoord && b.z < divisionCoord && c.z < divisionCoord ) divisionId = i;
				break;
			}
		}

		if ( p_depth >= _maxDepth ) return;
		if ( divisionId - p_firstTriangleId + 1 <= _minTrianglesPerLeaf
			 || p_lastTriangleId - divisionId <= _minTrianglesPerLeaf )
			return;

		// recursive calls
		_root->_left = new BVHNode();
		_buildRec( _root->_left, p_firstTriangleId, divisionId, p_depth + 1 );
		_root->_right = new BVHNode();
		_buildRec( _root->_right, divisionId+1, p_lastTriangleId, p_depth + 1 );
	}

	bool BVH::_intersectRec( const BVHNode * p_node,
							 const Ray &	 p_ray,
							 const float	 p_tMin,
							 const float	 p_tMax,
							 HitRecord &	 p_hitRecord ) const
	{
		/// TODO
		if ( p_node->_aabb.intersect( p_ray, p_tMin, p_tMax ) )
		{
			if ( p_node->isLeaf() )
			{
				float  tClosest = p_tMax;			  // Hit distance.
				size_t hitTri	= _triangles->size(); // Hit triangle id.
				float  u = 0.f, v = 0.f;
				for ( size_t i = p_node->_firstTriangleId; i <= p_node->_lastTriangleId; i++ )
				{
					float  t, utmp, vtmp;
					if ( ( *_triangles )[ i ].intersect( p_ray, t, utmp, vtmp ) )
					{
						if ( t >= p_tMin && t <= tClosest )
						{
							tClosest = t;
							hitTri	 = i;
							u		 = utmp;
							v		 = vtmp;
						}
					}
					if ( hitTri != _triangles->size() ) // Intersection found.
					{
						p_hitRecord._point	= p_ray.pointAtT( tClosest );
						p_hitRecord._normal = ( *_triangles )[ hitTri ].getInterpolatedFaceNormal( u, v );
						p_hitRecord.faceNormal( p_ray.getDirection() );
						p_hitRecord._distance = tClosest;
						p_hitRecord._object	  = _object;

						return true;
					}
				}
				return false;
			}
			bool left = false, right = false;
			HitRecord hrleft, hrright;
			if ( p_node->_left != nullptr )
			{
				left = _intersectRec( p_node->_left, p_ray, p_tMin, p_tMax, hrleft );
			}
			if ( p_node->_right != nullptr )
			{
				right = _intersectRec( p_node->_right, p_ray, p_tMin, p_tMax, hrright );
			}
			if ( left && right )
			{
				if ( hrleft._distance < hrright._distance ) {
					p_hitRecord._point	= hrleft._point;
					p_hitRecord._normal = hrleft._normal;
					p_hitRecord.faceNormal( p_ray.getDirection() );
					p_hitRecord._distance = hrleft._distance;
					p_hitRecord._object	  = hrleft._object;
				}
				else
				{
					p_hitRecord._point	= hrright._point;
					p_hitRecord._normal = hrright._normal;
					p_hitRecord.faceNormal( p_ray.getDirection() );
					p_hitRecord._distance = hrright._distance;
					p_hitRecord._object	  = hrright._object;
				}
			}
			return left || right;
		}
		return false;
	}
	bool BVH::_intersectAnyRec( const BVHNode * p_node,
								const Ray &		p_ray,
								const float		p_tMin,
								const float		p_tMax ) const
	{
		/// TODO
		if ( p_node->_aabb.intersect( p_ray, p_tMin, p_tMax ) )
		{
			if ( p_node->isLeaf() )
			{
				for ( size_t i = p_node->_firstTriangleId; i <= p_node->_lastTriangleId; i++ )
				{
					float t, u, v;
					if ( ( *_triangles )[ i ].intersect( p_ray, t, u, v ) )
					{
						if ( t >= p_tMin && t <= p_tMax ) return true; // No need to search for the nearest.
					}
				}
			}
			if ( p_node->_left != nullptr )
			{
				if ( _intersectAnyRec( p_node->_left, p_ray, p_tMin, p_tMax ) ) return true;
			}
			if ( p_node->_right != nullptr )
			{
				if ( _intersectAnyRec( p_node->_right, p_ray, p_tMin, p_tMax ) ) return true;
			}
		}
		return false;
	}
} // namespace RT_ISICG
