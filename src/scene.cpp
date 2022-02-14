#include "scene.hpp"
#include "materials/color_material.hpp"
#include "objects/sphere.hpp"
#include "objects/Plane.hpp"
#include "lights/PointLight.hpp"

namespace RT_ISICG
{
	Scene::Scene() { _addMaterial( new ColorMaterial( "default", WHITE ) ); }

	Scene::~Scene()
	{
		for ( const ObjectMapPair & object : _objectMap )
		{
			delete object.second;
		}
		for ( const MaterialMapPair & material : _materialMap )
		{
			delete material.second;
		}
		for ( const BaseLight * light : _lightList )
		{
			delete light;
		}
	}

	void Scene::init()
	{
		// Add objects.
		_addObject( new Sphere( "Sphere1", Vec3f( 0.f, 0.f, 3.f ), 1.f ) );
		_addObject( new Plane( "Plane1", Vec3f( 0.f, -2.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );

		// Add materials.
		_addMaterial( new ColorMaterial( "Blue", BLUE ) );
		_addMaterial( new ColorMaterial( "Red", RED ) );

		// Link objects and materials.
		_attachMaterialToObject( "Blue", "Sphere1" );
		_attachMaterialToObject( "Red", "Plane1" );

		// Add lighting
		_addLight( new PointLight( Vec3f( 1.f, 10.f, 1.f ), WHITE, 100.f ) );
	}

	bool Scene::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax, HitRecord & p_hitRecord ) const
	{
		float tMax = p_tMax;
		bool  hit  = false;
		for ( const ObjectMapPair & object : _objectMap )
		{
			if ( object.second->intersect( p_ray, p_tMin, tMax, p_hitRecord ) )
			{
				tMax = p_hitRecord._distance; // update tMax to conserve the nearest hit
				hit	 = true;
			}
		}
		return hit;
	}

	bool Scene::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		for ( const ObjectMapPair & object : _objectMap )
			if ( object.second->intersectAny( p_ray, p_tMin, p_tMax ) ) return true;
		return false;
	}

	void Scene::_addObject( BaseObject * p_object )
	{
		const std::string & name = p_object->getName();
		if ( _objectMap.find( name ) != _objectMap.end() )
		{
			std::cout << "[Scene::addObject] Object \'" << name << "\' already exists" << std::endl;
			delete p_object;
		}
		else
		{
			_objectMap[ name ] = p_object;
			_objectMap[ name ]->setMaterial( _materialMap[ "default" ] );
		}
	}

	void Scene::_addMaterial( BaseMaterial * p_material )
	{
		const std::string & name = p_material->getName();
		if ( _materialMap.find( name ) != _materialMap.end() )
		{
			std::cout << "[Scene::addMaterial] Material \'" << name << "\' already exists" << std::endl;
			delete p_material;
		}
		else
		{
			_materialMap[ name ] = p_material;
		}
	}

	void Scene::_addLight( BaseLight * p_light ) { _lightList.emplace_back( p_light ); }

	void Scene::_attachMaterialToObject( const std::string & p_materialName, const std::string & p_objectName )
	{
		if ( _objectMap.find( p_objectName ) == _objectMap.end() )
		{
			std::cout << "[Scene::attachMaterialToObject] Object \'" << p_objectName << "\' does not exist"
					  << std::endl;
		}
		else if ( _materialMap.find( p_materialName ) == _materialMap.end() )
		{
			std::cout << "[Scene::attachMaterialToObject] Material \'" << p_materialName << "\' does not exist, "
					  << "object \'" << p_objectName << "\' keeps its material \'"
					  << _objectMap[ p_objectName ]->getMaterial()->getName() << "\'" << std::endl;
		}
		else
		{
			_objectMap[ p_objectName ]->setMaterial( _materialMap[ p_materialName ] );
		}
	}

} // namespace RT_ISICG
