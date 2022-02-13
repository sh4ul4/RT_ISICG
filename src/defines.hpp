#ifndef __RT_ISICG_DEFINES__
#define __RT_ISICG_DEFINES__

#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include <iostream>
#include <limits>
#include <string>

namespace RT_ISICG
{
	// TODO: split this file

	// Scalars.
	const float PIf			 = glm::pi<float>();
	const float PI_2f		 = glm::half_pi<float>();
	const float PI_4f		 = glm::quarter_pi<float>();
	const float PI_32f		 = glm::three_over_two_pi<float>();
	const float TWO_PIf		 = glm::two_pi<float>();
	const float INV_PIf		 = glm::one_over_pi<float>();
	const float INV_2PIf	 = glm::one_over_two_pi<float>();
	const float FLT_INFINITY = std::numeric_limits<float>::infinity();

	const float SHADOW_EPSILON = 0.001f;

	// Vectors.
	using Vec2i = glm::ivec2;
	using Vec3i = glm::ivec3;
	using Vec4i = glm::ivec4;
	using Vec2f = glm::vec2;
	using Vec3f = glm::vec3;
	using Vec4f = glm::vec4;

	const Vec2i VEC2I_ZERO = Vec2i( 0 );
	const Vec3i VEC3I_ZERO = Vec3i( 0 );
	const Vec4i VEC4I_ZERO = Vec4i( 0 );
	const Vec2f VEC2F_ZERO = Vec2f( 0.f );
	const Vec3f VEC3F_ZERO = Vec3f( 0.f );
	const Vec4f VEC4F_ZERO = Vec4f( 0.f );

	// Matrices.
	using Mat3f = glm::mat3;
	using Mat4f = glm::mat4;

	const Mat4f MAT3F_ID = Mat3f();
	const Mat4f MAT4F_ID = Mat4f();

	// Colors are simply Vec3f
	const Vec3f BLACK( 0.f, 0.f, 0.f );
	const Vec3f WHITE( 1.f, 1.f, 1.f );
	const Vec3f GREY( 0.5f, 0.5f, 0.5f );
	const Vec3f RED( 1.f, 0.f, 0.f );
	const Vec3f GREEN( 0.f, 1.f, 0.f );
	const Vec3f BLUE( 0.f, 0.f, 1.f );
	const Vec3f YELLOW( 1.f, 1.f, 0.f );
	const Vec3f MAGENTA( 1.f, 0.f, 1.f );
	const Vec3f CYAN( 0.f, 1.f, 1.f );

	// Paths.
	const std::string RESULTS_PATH = "results/";

	// Utils.
	static inline float intAsFloat( const int p_i )
	{
		union
		{
			int	  a;
			float b;
		} u;
		u.a = p_i;
		return u.b;
	}
	static inline int floatAsInt( const float p_f )
	{
		union
		{
			float a;
			int	  b;
		} u;
		u.a = p_f;
		return u.b;
	}

} // namespace RT_ISICG

#endif // __RT_ISICG_DEFINES__
