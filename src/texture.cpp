#include "texture.hpp"
#include "io/stb_include.hpp"

#define MIPMAPS 0

namespace RT_ISICG
{
	Texture::Texture( const int p_width, const int p_height, const int _nbChannels )
		: _width( p_width ), _height( p_height ), _nbChannels( _nbChannels ),
		  _pixels( new unsigned char[ _width * _height * _nbChannels ] )
	{
	}
	Texture::Texture( const std::string & path )
	{
		stbi_uc * data = nullptr;
		data		   = stbi_load( path.c_str(), &_width, &_height, &_nbChannels, 0 );
		if ( data )
		{
			_pixels = new unsigned char[ _width * _height * _nbChannels ];
			for ( size_t i = 0; i < _width * _height * _nbChannels; i++ )
			{
				_pixels[ i ] = data[ i ];
			}
			stbi_image_free( data );
#if MIPMAPS
			// make mipmaps
			size_t len = 0;
			for ( int i = 0; i < _mipmaplevel; i++ )
			{
				const int p = pow( 2, i + 1 );
				const int w = (float)_width / (float)p;
				const int h = (float)_height / (float)p;
				mipmapsizes.emplace_back( w * h * _nbChannels, w, h );
				len += size_t( w * h * _nbChannels );
			}
			_mipmaps   = new unsigned char[ len ];
			size_t pos = 0;
			for ( int i = 0; i < _mipmaplevel; i++ )
			{
				const int w = mipmapsizes[ i ][ 1 ];
				const int h = mipmapsizes[ i ][ 2 ];
				for ( size_t x = 0; x < w; x++ )
				{
					for ( size_t y = 0; y < h; y++ )
					{
						int				   ipx = x * pow( 2, i + 1 );
						int				   ipy = y * pow( 2, i + 1 );
						std::vector<float> color {};
						for ( size_t j = 0; j < _nbChannels; j++ )
							color.emplace_back( 0.f );
						for ( size_t _x = 0; _x < _width / mipmapsizes[ i ][ 1 ]; _x++ )
						{
							for ( size_t _y = 0; _y < _width / mipmapsizes[ i ][ 1 ]; _y++ )
							{
								const int ip = ( ipx + _x ) + ( ipy + _y ) * _width;
								for ( int c = 0; c < _nbChannels; c++ )
									color[ c ] += (float)( _pixels[ ip * _nbChannels + c ] );
							}
						}
						for ( size_t c = 0; c < _nbChannels; c++ )
							_mipmaps[ pos + ( ( x + y * w ) * _nbChannels ) + c ]
								= unsigned char( color[ c ] / _nbChannels );
					}
				}
				pos += size_t( mipmapsizes[ i ][ 0 ] );
			}
#endif
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
	}
	void Texture::saveJPG( const std::string & p_path, const int p_quality )
	{
		stbi_write_jpg( p_path.c_str(), _width, _height, _nbChannels, _pixels, p_quality );
	}
} // namespace RT_ISICG
