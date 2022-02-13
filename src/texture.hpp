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
		Texture( const int p_width, const int p_height, const int _nbChannels = 3 )
			: _width( p_width ), _height( p_height ), _pixels( _width * _height * _nbChannels, 0 )
		{
			_pixels.shrink_to_fit();
		}
		~Texture() = default;

		inline const int						  getWidth() const { return _width; }
		inline const int						  getHeight() const { return _height; }
		inline std::vector<unsigned char> &		  getPixels() { return _pixels; }
		inline const std::vector<unsigned char> & getPixels() const { return _pixels; }

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

		void saveJPG( const std::string & p_path, const int p_quality = 100 );

	  private:
		const int _nbChannels = 3;
		int		  _width;
		int		  _height;

		std::vector<unsigned char> _pixels;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_IMAGE__
