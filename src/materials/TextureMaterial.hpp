#pragma once

#include "base_material.hpp"
#include "defines.hpp"

#define MIPMAPS 0
#define CONEMAPS 0

namespace RT_ISICG
{
	class TextureMaterial : public BaseMaterial
	{
	  public:
		TextureMaterial( const std::string & p_name, const std::string& path ) : BaseMaterial( p_name )
		{
			_texture = new Texture( path );
		}

		~TextureMaterial() { delete _texture; }

		bool isTextured() const override { return true; }

		void getDiskUvs( const float r, const Vec2f & p, std::vector<Vec2f> & uvs ) const
		{
			const float precision = 0.01f;
			for ( float w = 0.f; w < r * 2.f; w += precision )
			{
				for ( float h = 0.f; h < r * 2.f; h += precision )
				{
					const float dx = r - w;
					const float dy = r - h;
					if ( ( dx * dx + dy * dy ) <= ( r * r ) )
						uvs.emplace_back( std::abs( fmod( p.x + dx, 1.f ) ), std::abs( fmod( p.y + dy, 1.f ) ) );
				}
			}
		}

		Vec3f shade( const Ray & p_ray, const HitRecord & p_hit, const LightSample & p_lightSample ) const override
		{
			if ( _texture == nullptr || _texture->getPixels() == nullptr ) return GREEN; // return noticeable color
			const Vec2f uv
				= Vec2f( std::abs( fmod( p_hit._uv.x, 1.f ) ), std::abs( fmod( p_hit._uv.y, 1.f ) ) ); // get u and v
			Vec3f pixel( 0.f );
#if MIPMAPS
			const float w = _texture->getWidth();
			const float h = _texture->getHeight();
			const float c = p_hit._pixelConeRad;
			const Vec3f tf
				= Vec3f( glm::distance( Vec2f( p_hit._textureFootprint[ 0 ] * w, p_hit._textureFootprint[ 1 ] * h ),
										Vec2f( p_hit._textureFootprint[ 2 ] * w, p_hit._textureFootprint[ 3 ] * h ) ),
						 glm::distance( Vec2f( p_hit._textureFootprint[ 4 ] * w, p_hit._textureFootprint[ 5 ] * h ),
										Vec2f( p_hit._textureFootprint[ 2 ] * w, p_hit._textureFootprint[ 3 ] * h ) ),
						 glm::distance( Vec2f( p_hit._textureFootprint[ 0 ] * w, p_hit._textureFootprint[ 1 ] * h ),
										Vec2f( p_hit._textureFootprint[ 4 ] * w, p_hit._textureFootprint[ 5 ] * h ) ) );
			const float semiPerim = ( tf.x + tf.y + tf.z ) / 2.f;
			const float area  = sqrtf( semiPerim * ( semiPerim - tf.x ) * ( semiPerim - tf.y ) * ( semiPerim - tf.z ) );
			const int	mmlvl = glm::clamp( (int)( p_hit._pixelConeRad * 10000 / area ), 0, 4 );
			_texture->getMipMapPixel(
				mmlvl, uv.x * ( _texture->getWidth() - 1 ), uv.y * ( _texture->getHeight() - 1 ), pixel );
			//pixel = Vec3f( mmlvl * 63.75 );
#elif CONEMAPS
			std::vector<Vec2f> uvs;
			getDiskUvs( p_hit._pixelConeRad, uv, uvs );
			for ( Vec2f & _uv : uvs )
			{
				Vec3f pix( 0.f );
				_texture->getPixel( _uv.x * ( _texture->getWidth() - 1 ), _uv.y * ( _texture->getHeight() - 1 ), pix );
				pixel += pix;
			}
			if ( uvs.size() > 0 ) pixel /= uvs.size();
#else
			_texture->getPixel( uv.x * ( _texture->getWidth() - 1 ), uv.y * ( _texture->getHeight() - 1 ), pixel );
#endif
			return pixel / 255.f;
		}

		inline const Vec3f & getFlatColor() const override { return BLACK; }

	  protected:
		Texture * _texture = nullptr;
	};

} // namespace RT_ISICG
