#ifndef __RT_ISICG_IMAGE__
#define __RT_ISICG_IMAGE__

#include "defines.hpp"
#include <string>
#include <vector>

namespace RT_ISICG
{
	class Texture
	{
	  public:
		Texture() = delete;
		Texture( const int p_width, const int p_height, const int _nbChannels = 3 );
		Texture( const std::string & path );
		~Texture()
		{
			if ( _pixels ) delete[] _pixels;
			if ( _mipmaps ) delete[] _mipmaps;
		}

		inline const int						  getWidth() const { return _width; }
		inline const int						  getHeight() const { return _height; }
		inline unsigned char*		  getPixels() { return _pixels; }
		inline const unsigned char *			  getPixels() const { return _pixels; }

		void setPixel( const int p_i, const int p_j, const Vec3f & p_color )
		{
			assert( _nbChannels == 3 );
			const int pixelId	   = ( p_i + p_j * _width ) * _nbChannels;
			_pixels[ pixelId ]	   = static_cast<unsigned char>( p_color.r * 255 );
			_pixels[ pixelId + 1 ] = static_cast<unsigned char>( p_color.g * 255 );
			_pixels[ pixelId + 2 ] = static_cast<unsigned char>( p_color.b * 255 );
		}

		void setPixel( const int p_i, const int p_j, const Vec4f & p_color )
		{
			assert( _nbChannels == 4 );
			const int pixelId	   = ( p_i + p_j * _width ) * _nbChannels;
			_pixels[ pixelId ]	   = static_cast<unsigned char>( p_color.r * 255 );
			_pixels[ pixelId + 1 ] = static_cast<unsigned char>( p_color.g * 255 );
			_pixels[ pixelId + 2 ] = static_cast<unsigned char>( p_color.b * 255 );
			_pixels[ pixelId + 3 ] = static_cast<unsigned char>( p_color.b * 255 );
		}

		void getMipMapPixel( const int p_mipmaplevel, const int p_i, const int p_j, Vec3f & p_pixel )
		{
			int pixelId = 0;
			for ( int i = 0; i < p_mipmaplevel; i++ )
			{
				pixelId += mipmapsizes[ i ][ 0 ];
			}
			int w  = mipmapsizes[ p_mipmaplevel ][ 1 ];
			int h  = mipmapsizes[ p_mipmaplevel ][ 2 ];
			int pi = (float)( float( p_i ) / float( _width ) ) * float( w );
			int pj = (float)( float( p_j ) / float( _height ) ) * float( h );
			pixelId += ( pi + ( ( h - 1 ) - pj ) * w ) * _nbChannels;
			p_pixel.r = static_cast<float>( _mipmaps[ pixelId ] );
			p_pixel.g = static_cast<float>( _mipmaps[ pixelId + 1 ] );
			p_pixel.b = static_cast<float>( _mipmaps[ pixelId + 2 ] );
		}

		void getPixel( const int p_i, const int p_j, Vec3f & p_pixel )
		{
			assert( _pixels != nullptr && (_nbChannels == 3 || _nbChannels == 4) );
			const int pixelId = ( p_i + ( ( _height - 1) - p_j ) * _width ) * _nbChannels;
			p_pixel.r		  = static_cast<float>( _pixels[ pixelId ]);
			p_pixel.g		  = static_cast<float>( _pixels[ pixelId +1]);
			p_pixel.b		  = static_cast<float>( _pixels[ pixelId +2]);
		}

		void getPixel( const int p_i, const int p_j, Vec4f & p_pixel )
		{
			assert( _nbChannels == 4 );
			const int pixelId	   = ( p_i + ( ( _width - 1 ) - p_j ) * _width ) * _nbChannels;
			p_pixel.r			   = static_cast<float>( _pixels[ pixelId ] );
			p_pixel.g			   = static_cast<float>( _pixels[ pixelId + 1 ] );
			p_pixel.b			   = static_cast<float>( _pixels[ pixelId + 2 ] );
			p_pixel.a			   = static_cast<float>( _pixels[ pixelId + 3 ] );
		}

		void saveJPG( const std::string & p_path, const int p_quality = 100 );

	  private:
		int _nbChannels;
		int		  _width;
		int		  _height;
		int		  _mipmaplevel = 5;
		std::vector<Vec3i> mipmapsizes;

		unsigned char * _pixels = nullptr;
		unsigned char * _mipmaps = nullptr;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_IMAGE__
