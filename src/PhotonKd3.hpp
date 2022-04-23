#pragma once

#include "defines.hpp"
#include <vector>
#include <algorithm>

namespace RT_ISICG
{
	class PhotonKd3
	{
	  public:
		class Photon
		{
		  public:
			Photon() = delete;
			Photon( const Vec3f & pos, const Vec3f & pow ) : pos( pos ), pow( pow ) {}
			Photon( const Photon & other ) : pos( other.pos ), pow( other.pow ) {}
			Vec3f pos; // xyz position
			Vec3f pow; // rgb power
		};
		class Node
		{
		  public:
			Node() = delete;
			Node( const Photon & p ) : p( p ) {}
			~Node()
			{
				if ( left != nullptr ) delete left;
				if ( right != nullptr ) delete right;
			}
			Node * left	 = nullptr;
			Node * right = nullptr;
			Photon p;
		};
		~PhotonKd3() { delete root; }
		std::vector<Photon> photons;

	  private:
		Node *				root = nullptr;
		bool				built = false;
		static bool				comp( const Vec3f & l, const Vec3f & r, const uint8_t dim ) { return l[ dim ] < r[ dim ]; }
		Node *				buildRec( Node * node, const size_t min, const size_t max, const uint8_t dim )
		{
			const size_t len = max - min;
			std::sort( photons.begin() + min,
					   photons.begin() + max,
					   [dim]( const Photon & l, const Photon & r ) { return comp( l.pos, r.pos, dim ); } );
			if ( len == 1 )
			{
				node = new Node( photons[ min ] );
				return node;
			}
			else if ( len == 2 )
			{
				node	   = new Node( photons[ min + 1 ] );
				node->left = new Node( photons[ min ] );
				return node;
			}
			else
			{
				size_t mid	= min + ( max - min ) / 2;
				node		= new Node( photons[ mid ] );
				node->left	= buildRec( node->left, min, mid, ( dim + 1 ) % 3 );
				node->right = buildRec( node->right, mid + 1, max, ( dim + 1 ) % 3 );
				return node;
			}
		}

	  public:
		void build()
		{
			if ( photons.size() == 0 )
			{
				std::cout << "no photons" << std::endl;
				return;
			}
			std::cout << "start build " << photons.size() << " photons" << std::endl;
			root = buildRec( root, 0, photons.size(), 0 );
			std::cout << "end build" << std::endl;
			built = true;
		}

	  private:
		bool contains( Node * node, const std::vector<Node *> & knearest ) const
		{
			for ( size_t i = 0; i < knearest.size(); i++ )
			{
				if ( knearest[ i ]->p.pos == node->p.pos ) return true;
			}
			return false;
		}
		float getMinDistVec( const Vec3f & pos, const std::vector<Node *> & knearest ) const
		{
			float min = INFINITY;
			for ( auto n : knearest )
			{
				float dist = glm::distance( pos, n->p.pos );
				if ( dist < min ) min = dist;
			}
			return min;
		}
		float getMaxDistVec( const Vec3f & pos, const std::vector<Node *> & knearest ) const
		{
			if ( knearest.size() == 0 ) return INFINITY;
			float max = 0;
			for ( auto n : knearest )
			{
				float dist = glm::distance( pos, n->p.pos );
				if ( dist > max ) max = dist;
			}
			return max;
		}
		void removeFarthest( const Vec3f & pos, std::vector<Node *> & knearest ) const
		{
			float  max = 0;
			int it  = 0;
			for ( int i = 0; i < knearest.size(); i++ )
			{
				float dist = glm::distance( pos, knearest[ i ]->p.pos );
				if ( dist >= max )
				{
					max = dist;
					it	= i;
				}
			}
			knearest.erase( knearest.begin() + it );
		}
		void knnRec( Node * root, const Vec3f & point, size_t index, const int k, std::vector<Node *>& knearest, float& maxBestDist, float& minBestDist ) const
		{
			if ( root == nullptr ) return;
			const float d = glm::distance( root->p.pos, point );
			if ( d < getMaxDistVec( point, knearest ) && !contains( root, knearest ) )
			{
				if ( knearest.size() == k ) { removeFarthest( point, knearest ); }
				knearest.emplace_back( root );
			}
			maxBestDist = getMaxDistVec( point, knearest );
			minBestDist = getMinDistVec( point, knearest );
			if ( maxBestDist == 0.f ) return;
			const float dx = root->p.pos[ index ] - point[ index ];
			index	 = ( index + 1 ) % 3;
			knnRec( dx > 0.f ? root->left : root->right, point, index, k, knearest, maxBestDist, minBestDist );
			if ( dx * dx >= minBestDist ) return;
			knnRec( dx > 0.f ? root->right : root->left, point, index, k, knearest, maxBestDist, minBestDist );
		}

	  public:
		//std::vector<Node *> knearest;
		void knn( const Vec3f & point, const int k, std::vector<Node *> & knearest ) const
		{
			if ( !built ) return;
			//knearest.clear();
			float maxBestDist = INFINITY;
			float minBestDist = INFINITY;
			knnRec( root, point, 0, k, knearest, maxBestDist, minBestDist );
		}
	};
} // namespace RT_ISICG