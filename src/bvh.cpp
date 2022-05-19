#include "bvh.hpp"
#include "geometry/triangle_mesh_geometry.hpp"
#include "utils/chrono.hpp"
#include <algorithm>

//#define SAH

namespace RT_ISICG
{
	void BVH::build( std::vector<TriangleMeshGeometry> * p_triangles )
	{
		std::cout << "Building BVH..." << std::endl;
		if ( p_triangles == nullptr || p_triangles->empty() )
		{
			throw std::exception( "BVH::build() error: no triangle provided" );
		}
		_triangles = p_triangles;

		Chrono chr;
		chr.start();

		_root = new BVHNode();
		_buildRec( _root, 0, (unsigned int)_triangles->size(), 0 );

		chr.stop();

		std::cout << "[DONE]: " << chr.elapsedTime() << "s" << std::endl;
	}

	bool BVH::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax, HitRecord & p_hitRecord ) const
	{
		if ( _root != nullptr ) return _intersectRec( _root, p_ray, p_tMin, p_tMax, p_hitRecord );
		return false;
	}

	bool BVH::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		if ( _root != nullptr ) return _intersectAnyRec( _root, p_ray, p_tMin, p_tMax );
		return false;
	}
#ifdef SAH
	float predictSahCost(const float traversalCost, const float intersectCost, const int nbTriA, const int nbTriB, const float surfaceA, const float surfaceB) {
		Vec2f pAB( /*glm::normalize*/( Vec2f( surfaceA, surfaceB ) ) );
		return traversalCost + pAB.x * nbTriA * intersectCost + pAB.y * nbTriB * intersectCost;
	}

	inline float cuboidSurface( const float w, const float h, const float l )
	{
		return 2.f * ( l * w + w * h + l * h );
	}

	void BVH::_buildRec( BVHNode *			p_node,
						 const unsigned int p_firstTriangleId,
						 const unsigned int p_lastTriangleId,
						 const unsigned int p_depth )
	{
		p_node->_firstTriangleId = p_firstTriangleId;
		p_node->_lastTriangleId	 = p_lastTriangleId;
		// make AABB
		for ( unsigned int i = p_firstTriangleId; i < p_lastTriangleId; i++ )
		{
			Vec3f a, b, c;
			( *_triangles )[ i ].getVertices( a, b, c );
			p_node->_aabb.extend( a );
			p_node->_aabb.extend( b );
			p_node->_aabb.extend( c );
		}
		// early exit conditions
		if ( p_lastTriangleId - p_firstTriangleId < _minTrianglesPerLeaf ) return;
		if ( p_depth > _maxDepth ) return;
		// partition triangles between p_firstTriangleId and p_lastTriangleId
		std::vector<TriangleMeshGeometry>::iterator partIterator = _triangles->begin() + p_firstTriangleId;
		const size_t axis		  = p_node->_aabb.largestAxis();
		if ( axis > 10 )
		{
			const float									binnings	 = 200;
			const float									minSahCost	 = -INFINITY;
			for ( float i = 0; i < binnings; i++ )
			{
				const float factor	= i / binnings;
				const float axisMid = p_node->_aabb.getMin()[ axis ]
									  + factor * ( p_node->_aabb.getMax()[ axis ] - p_node->_aabb.getMin()[ axis ] );
				const float surface = cuboidSurface( p_node->_aabb.getMax()[ 0 ] - p_node->_aabb.getMin()[ 0 ],
													 p_node->_aabb.getMax()[ 1 ] - p_node->_aabb.getMin()[ 1 ],
													 p_node->_aabb.getMax()[ 2 ] - p_node->_aabb.getMin()[ 2 ] );
				const float surfaceA
					= cuboidSurface(
						  axisMid - p_node->_aabb.getMin()[ axis ],
						  p_node->_aabb.getMax()[ ( axis + 1 ) % 3 ] - p_node->_aabb.getMin()[ ( axis + 1 ) % 3 ],
						  p_node->_aabb.getMax()[ ( axis + 2 ) % 3 ] - p_node->_aabb.getMin()[ ( axis + 2 ) % 3 ] )
					  / surface;
				const float surfaceB
					= cuboidSurface(
						  p_node->_aabb.getMax()[ axis ] - axisMid,
						  p_node->_aabb.getMax()[ ( axis + 1 ) % 3 ] - p_node->_aabb.getMin()[ ( axis + 1 ) % 3 ],
						  p_node->_aabb.getMax()[ ( axis + 2 ) % 3 ] - p_node->_aabb.getMin()[ ( axis + 2 ) % 3 ] )
					  / surface;
				const auto currentPartIterator
					= std::partition( _triangles->begin() + p_firstTriangleId,
									  _triangles->begin() + p_lastTriangleId,
									  [ = ]( const TriangleMeshGeometry & tmg )
									  {
										  Vec3f a, b, c;
										  tmg.getVertices( a, b, c );
										  switch ( axis )
										  {
										  case 0: return ( a.x < axisMid && b.x < axisMid && c.x < axisMid ); break;
										  case 1: return ( a.y < axisMid && b.y < axisMid && c.y < axisMid ); break;
										  case 2: return ( a.z < axisMid && b.z < axisMid && c.z < axisMid ); break;
										  default: break;
										  }
									  } );
				const int	nbTriA		   = currentPartIterator - _triangles->begin() + p_firstTriangleId;
				const int	nbTriB		   = _triangles->begin() + p_lastTriangleId - currentPartIterator;
				const float currentSahCost = predictSahCost( 1.f, 2.f, nbTriA, nbTriB, surfaceA, surfaceB );
				if ( currentSahCost < 0 )
				{
					std::cout << factor << std::endl;
					std::cout << axisMid << std::endl;
					std::cout << surfaceA << std::endl;
					std::cout << surfaceB << std::endl;
					std::cout << nbTriA << std::endl;
					std::cout << nbTriB << std::endl << std::endl;
				}
				if ( currentSahCost > minSahCost || partIterator == _triangles->begin() + p_firstTriangleId )
				{
					partIterator = currentPartIterator;
				}
			}
		}
		else
		{
			const float axisMid = ( p_node->_aabb.getMin()[ axis ] + p_node->_aabb.getMax()[ axis ] ) / 2.f;
			partIterator
				= std::partition( _triangles->begin() + p_firstTriangleId,
								  _triangles->begin() + p_lastTriangleId,
								  [ = ]( const TriangleMeshGeometry & tmg )
								  {
									  Vec3f a, b, c;
									  tmg.getVertices( a, b, c );
									  switch ( axis )
									  {
									  case 0: return ( a.x < axisMid && b.x < axisMid && c.x < axisMid ); break;
									  case 1: return ( a.y < axisMid && b.y < axisMid && c.y < axisMid ); break;
									  case 2: return ( a.z < axisMid && b.z < axisMid && c.z < axisMid ); break;
									  default: break;
									  }
								  } );
		}

		const unsigned int partId = (unsigned int)( partIterator - _triangles->begin() );
		// recursive calls
		p_node->_left  = new BVHNode();
		p_node->_right = new BVHNode();
		_buildRec( p_node->_left, p_firstTriangleId, partId, p_depth + 1 );
		_buildRec( p_node->_right, partId, p_lastTriangleId, p_depth + 1 );
	}

#else
	void BVH::_buildRec( BVHNode *			p_node,
						 const unsigned int p_firstTriangleId,
						 const unsigned int p_lastTriangleId,
						 const unsigned int p_depth )
	{
		p_node->_firstTriangleId = p_firstTriangleId;
		p_node->_lastTriangleId	 = p_lastTriangleId;
		// make AABB
		for ( unsigned int i = p_firstTriangleId; i < p_lastTriangleId; i++ )
		{
			Vec3f a, b, c;
			( *_triangles )[ i ].getVertices( a, b, c );
			p_node->_aabb.extend( a );
			p_node->_aabb.extend( b );
			p_node->_aabb.extend( c );
		}
		// early exit conditions
		if ( p_lastTriangleId - p_firstTriangleId < _minTrianglesPerLeaf ) return;
		if ( p_depth > _maxDepth ) return;
		// partition triangles between p_firstTriangleId and p_lastTriangleId
		const size_t axis		  = p_node->_aabb.largestAxis();
		const float	 axisMid	  = ( p_node->_aabb.getMin()[ axis ] + p_node->_aabb.getMax()[ axis ] ) / 2.f;
		const auto	 partIterator = std::partition( _triangles->begin() + p_firstTriangleId,
													_triangles->begin() + p_lastTriangleId,
													[ = ]( const TriangleMeshGeometry & tmg )
													{
														Vec3f a, b, c;
														tmg.getVertices( a, b, c );
														switch ( axis )
														{
														case 0:
															return ( a.x < axisMid && b.x < axisMid && c.x < axisMid );
															break;
														case 1:
															return ( a.y < axisMid && b.y < axisMid && c.y < axisMid );
															break;
														case 2:
															return ( a.z < axisMid && b.z < axisMid && c.z < axisMid );
															break;
														default: break;
														}
													} );

		const unsigned int partId = (unsigned int)( partIterator - _triangles->begin() );
		// recursive calls
		p_node->_left  = new BVHNode();
		p_node->_right = new BVHNode();
		_buildRec( p_node->_left, p_firstTriangleId, partId, p_depth + 1 );
		_buildRec( p_node->_right, partId, p_lastTriangleId, p_depth + 1 );
	}
#endif

	bool BVH::_intersectRec( const BVHNode * p_node,
							 const Ray &	 p_ray,
							 const float	 p_tMin,
							 const float	 p_tMax,
							 HitRecord &	 p_hitRecord ) const
	{
		if ( p_node->_aabb.intersect( p_ray, p_tMin, p_tMax ) )
		{
			if ( p_node->isLeaf() )
			{
				float		 tClosest = p_tMax;
				unsigned int hitTri	  = p_node->_lastTriangleId;
				float		 u = 0.f, v = 0.f;
				for ( unsigned int i = p_node->_firstTriangleId; i < p_node->_lastTriangleId; i++ )
				{
					float t, utmp, vtmp;
					if ( ( *_triangles )[ i ].intersect( p_ray, t, utmp, vtmp ) )
					{
						if ( t >= p_tMin && t < tClosest && t <= p_tMax )
						{
							tClosest = t;
							hitTri	 = i;
							u		 = utmp;
							v		 = vtmp;
						}
					}
				}
				if ( hitTri != p_node->_lastTriangleId )
				{
					p_hitRecord._point	  = p_ray.pointAtT( tClosest );
					p_hitRecord._distance = tClosest;
					p_hitRecord._normal	  = ( *_triangles )[ hitTri ].getInterpolatedFaceNormal( u, v );
					p_hitRecord.faceNormal( p_ray.getDirection() );
					p_hitRecord._uv = ( *_triangles )[ hitTri ].getInterpolatedTextureCoords( u, v );
					p_hitRecord._pixelConeRad += p_hitRecord._distance * glm::tan( p_hitRecord._pixelConeAlpha );
					( *_triangles )[ hitTri ].getUvs( p_hitRecord._textureFootprint );
					return true;
				}
			}
			else
			{
				HitRecord l, r;
				l._distance				  = INFINITY;
				r._distance				  = INFINITY;
				l._pixelConeRad	  = p_hitRecord._pixelConeRad;
				l._pixelConeAlpha		  = p_hitRecord._pixelConeAlpha;
				r._pixelConeRad			  = p_hitRecord._pixelConeRad;
				r._pixelConeAlpha		  = p_hitRecord._pixelConeAlpha;
				const bool intersectLeft  = _intersectRec( p_node->_left, p_ray, p_tMin, p_tMax, l );
				const bool intersectRight = _intersectRec( p_node->_right, p_ray, p_tMin, p_tMax, r );
				if ( intersectLeft || intersectRight )
				{
					p_hitRecord = ( l._distance < r._distance ) ? l : r;
					return true;
				}
			}
		}
		return false;
	}
	bool BVH::_intersectAnyRec( const BVHNode * p_node,
								const Ray &		p_ray,
								const float		p_tMin,
								const float		p_tMax ) const
	{
		if ( p_node->_aabb.intersect( p_ray, p_tMin, p_tMax ) )
		{
			if ( p_node->isLeaf() )
			{
				for ( size_t i = p_node->_firstTriangleId; i < p_node->_lastTriangleId; i++ )
				{
					float t, u, v;
					if ( ( *_triangles )[ i ].intersect( p_ray, t, u, v ) )
					{
						if ( t >= p_tMin && t <= p_tMax ) return true; // No need to search for the nearest.
					}
				}
				return false;
			}
			else
			{
				if ( _intersectAnyRec( p_node->_left, p_ray, p_tMin, p_tMax ) ) return true;
				if ( _intersectAnyRec( p_node->_right, p_ray, p_tMin, p_tMax ) ) return true;
			}
		}
		return false;
	}
} // namespace RT_ISICG
