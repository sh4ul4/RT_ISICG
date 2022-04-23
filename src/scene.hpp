#ifndef __RT_ISICG_SCENE__
#define __RT_ISICG_SCENE__

#include "defines.hpp"
#include "lights/base_light.hpp"
#include "objects/base_object.hpp"
#include <exception>
#include <map>
#include <vector>

namespace RT_ISICG
{
	using ObjectMap		  = std::map<const std::string, BaseObject *>;
	using ObjectMapPair	  = ObjectMap::value_type;
	using MaterialMap	  = std::map<const std::string, BaseMaterial *>;
	using MaterialMapPair = MaterialMap::value_type;
	using LightList		  = std::vector<BaseLight *>;

	class Scene
	{
		void tp0( Vec3f & cameraPosition, Vec3f & cameraLookAt );
		void tp1( Vec3f & cameraPosition, Vec3f & cameraLookAt );
		void tp2( Vec3f & cameraPosition, Vec3f & cameraLookAt );
		void tp3( Vec3f & cameraPosition, Vec3f & cameraLookAt );
		void tp4( Vec3f & cameraPosition, Vec3f & cameraLookAt );
		void tp5( Vec3f & cameraPosition, Vec3f & cameraLookAt );
		void tp6( Vec3f & cameraPosition, Vec3f & cameraLookAt );
		void tp7( Vec3f & cameraPosition, Vec3f & cameraLookAt );
		void lenses( Vec3f & cameraPosition, Vec3f & cameraLookAt );
		void caustics( Vec3f & cameraPosition, Vec3f & cameraLookAt );

	  public:
		Scene();
		~Scene();

		// Hard coded initialization.
		void init( Vec3f & cameraPosition, Vec3f & cameraLookAt );

		// Initialization from file.
		void init( const std::string & p_path ) { throw std::exception( "Not implemented !" ); }

		void loadFileTriangleMesh( const std::string & p_name, const std::string & p_path );

		const LightList & getLights() const { return _lightList; }

		// Check for nearest intersection between p_tMin and p_tMax : if found fill p_hitRecord.
		bool intersect( const Ray & p_ray, const float p_tMin, const float p_tMax, HitRecord & p_hitRecord ) const;

		// Check for nearest intersection between p_tMin and p_tMax : if found fill p_hitRecord.
		bool intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const;

	  private:
		void _addObject( BaseObject * p_object );
		void _addMaterial( BaseMaterial * p_material );
		void _addLight( BaseLight * p_light );
		void _addGlobeLightObject( const std::string & p_name,
								   const Vec3f &	   p_position,
								   const float		   p_radius,
								   const Vec3f &	   p_objectColor,
								   const Vec3f &	   p_lightColor,
								   const float		   p_power );

		void _attachMaterialToObject( const std::string & p_materialName, const std::string & p_objectName );

	  private:
		ObjectMap	_objectMap;
		MaterialMap _materialMap;
		LightList	_lightList;
	};
} // namespace RT_ISICG

#endif // __RT_ISICG_SCENE__
