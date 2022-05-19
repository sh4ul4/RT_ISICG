#include "triangle_mesh_geometry.hpp"
#include "objects/triangle_mesh.hpp"
#include <array>

//#define CULLING

namespace RT_ISICG
{
	TriangleMeshGeometry::TriangleMeshGeometry( const unsigned int p_v0,
												const unsigned int p_v1,
												const unsigned int p_v2,
												MeshTriangle *	   p_refMesh )
		: _v0( p_v0 ), _v1( p_v1 ), _v2( p_v2 ), _refMesh( p_refMesh )
	{
		_faceNormal = glm::normalize( glm::cross( _refMesh->_vertices[ p_v1 ] - _refMesh->_vertices[ p_v0 ],
												  _refMesh->_vertices[ p_v2 ] - _refMesh->_vertices[ p_v0 ] ) );
	}

	const Vec3f & TriangleMeshGeometry::getInterpolatedFaceNormal( const float u, const float v ) const
	{
		return ( 1.f - u - v ) * _refMesh->_normals[ _v0 ] + u * _refMesh->_normals[ _v1 ] + v * _refMesh->_normals[ _v2 ];
	}

	const Vec2f & TriangleMeshGeometry::getInterpolatedTextureCoords( const float u, const float v ) const
	{
		return ( 1.f - u - v ) * _refMesh->_uvs[ _v0 ] + u * _refMesh->_uvs[ _v1 ] + v * _refMesh->_uvs[ _v2 ];
	}

	void TriangleMeshGeometry::getUvs( std::array<float, 6> & arr) const
	{
		arr = { _refMesh->_uvs[ _v0 ].x, _refMesh->_uvs[ _v0 ].y, _refMesh->_uvs[ _v1 ].x,
				_refMesh->_uvs[ _v1 ].y, _refMesh->_uvs[ _v2 ].x, _refMesh->_uvs[ _v2 ].y };
	}

	void TriangleMeshGeometry::getVertices( Vec3f & a, Vec3f & b, Vec3f & c ) const
	{
		a = _refMesh->_vertices[ _v0 ];
		b = _refMesh->_vertices[ _v1 ];
		c = _refMesh->_vertices[ _v2 ];
	}

	bool TriangleMeshGeometry::intersect( const Ray & p_ray, float & p_t, float & p_u, float & p_v ) const
	{
		const Vec3f & o	 = p_ray.getOrigin();
		const Vec3f & d	 = glm::normalize(p_ray.getDirection());
		const Vec3f & v0 = _refMesh->_vertices[ _v0 ];
		const Vec3f & v1 = _refMesh->_vertices[ _v1 ];
		const Vec3f & v2 = _refMesh->_vertices[ _v2 ];

		/// TODO

		Vec3f  edge1 = v1 - v0;
		Vec3f  edge2 = v2 - v0;
		Vec3f  pvec	 = glm::cross( d, edge2 );
		float det			 = glm::dot( edge1, pvec );
#ifdef CULLING
		if ( det < 0.00001f ) return false;
		Vec3f tvec = o - v0;
		p_u		   = glm::dot( tvec, pvec );
		if ( p_u < 0.f || p_u > det ) return false;
		Vec3f qvec = glm::cross( tvec, edge1 );
		p_v		   = glm::dot( d, qvec );
		if ( p_v < 0.f || p_u + p_v > det ) return false;
		p_t = glm::dot( edge2, qvec );
		float inv_det = 1.f / det;
		p_t *= inv_det;
		p_u *= inv_det;
		p_v *= inv_det;
#else
		if ( det > -0.00001f && det < 0.00001f ) return false;
		float inv_det	   = 1.f / det;
		Vec3f tvec = o - v0;
		p_u		   = glm::dot( tvec, pvec ) * inv_det;
		if ( p_u < 0.f || p_u > 1.f ) return false;
		Vec3f qvec = glm::cross( tvec, edge1 );
		p_v		   = glm::dot( d, qvec ) * inv_det;
		if ( p_v < 0.f || p_u + p_v > 1.f ) return false;
		p_t		= glm::dot( edge2, qvec ) * inv_det;
#endif
		return true;
	}

} // namespace RT_ISICG
