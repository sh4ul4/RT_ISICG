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
			Vec3f pow; // rgb power-flux
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
		Node *		root  = nullptr;
		bool		built = false;
		static bool comp( const Vec3f & l, const Vec3f & r, const uint8_t dim ) { return l[ dim ] < r[ dim ]; }
		Node *		buildRec( Node * node, const size_t min, const size_t max, const uint8_t dim )
		{
			const size_t len = max - min;
			// sort the vector segment to balance the tree branch according to the given dimension
			std::sort( photons.begin() + min,
					   photons.begin() + max,
					   [ dim ]( const Photon & l, const Photon & r ) { return comp( l.pos, r.pos, dim ); } );
			// only one photon left
			if ( len == 1 )
			{
				node = new Node( photons[ min ] );
				return node;
			}
			// only two photons "left" :)
			else if ( len == 2 )
			{
				node	   = new Node( photons[ min + 1 ] );
				node->left = new Node( photons[ min ] );
				return node;
			}
			// enough photons left to create at least to branch-nodes
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
		// node is inside vector
		bool contains( const Node * node, const std::vector<Node *> & knearest ) const
		{
			for ( size_t i = 0; i < knearest.size(); i++ )
			{
				if ( knearest[ i ]->p.pos == node->p.pos ) return true;
			}
			return false;
		}

		// get minimum distance between pos and every photon in the vector
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

		// get maximum distance between pos and every photon in the vector
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

		// remove most distant photon from vector
		void removeFarthest( const Vec3f & pos, std::vector<Node *> & knearest ) const
		{
			float max = 0;
			int	  it  = 0;
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

		void knnRec( Node *				   root,
					 const Vec3f &		   point,
					 size_t				   index,
					 const int			   k,
					 std::vector<Node *> & knearest,
					 float &			   maxBestDist,
					 float &			   minBestDist ) const
		{
			if ( root == nullptr ) return;
			const float d = glm::distance( root->p.pos, point );
			// choose wether or not to add the current photon
			if ( d < getMaxDistVec( point, knearest ) && !contains( root, knearest ) )
			{
				if ( knearest.size() == k ) { removeFarthest( point, knearest ); }
				knearest.emplace_back( root );
			}
			// update min/max
			maxBestDist = getMaxDistVec( point, knearest );
			minBestDist = getMinDistVec( point, knearest );
			// early exit
			if ( maxBestDist == 0.f ) return;
			// set delta-dim
			const float ddim = root->p.pos[ index ] - point[ index ];
			// update dim
			index = ( index + 1 ) % 3;
			// recursive calls
			knnRec( ddim > 0.f ? root->left : root->right, point, index, k, knearest, maxBestDist, minBestDist );
			if ( ddim * ddim >= minBestDist ) return;
			knnRec( ddim > 0.f ? root->right : root->left, point, index, k, knearest, maxBestDist, minBestDist );
		}

	  public:
		void knn( const Vec3f & point, const int k, std::vector<Node *> & knearest ) const
		{
			if ( !built ) return;
			float maxBestDist = INFINITY;
			float minBestDist = INFINITY;
			knnRec( root, point, 0, k, knearest, maxBestDist, minBestDist );
		}
	};
} // namespace RT_ISICG