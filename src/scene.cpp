#include "scene.hpp"
#include "materials/color_material.hpp"
#include "objects/sphere.hpp"
#include "objects/Plane.hpp"
#include "objects/triangle_mesh.hpp"
#include "objects/SphereLightObject.hpp"
#include "objects/implicit_surface.hpp"
#include "objects/ImplicitSphere.hpp"
#include "objects/ImplicitPlane.hpp"
#include "objects/ImplicitFractal.hpp"
#include "objects/ImplicitIntersection.hpp"
#include "objects/ImplicitCuboid.hpp"
#include "objects/ImplicitInterpolation.hpp"
#include "objects/ImplicitSubstraction.hpp"
#include "lights/PointLight.hpp"
#include "lights/QuadLight.hpp"
#include "lights/TriLight.hpp"
#include "lights/GlobeLight.hpp"
#include "materials/lambert_material.hpp"
#include "materials/Plasticmaterial.hpp"
#include "materials/MatteMaterial.hpp"
#include "materials/RealisticMaterial.hpp"
#include "materials/MirrorMaterial.hpp"
#include "materials/TransparentMaterial.hpp"
#include "materials/EmissiveMaterial.hpp"
#include "materials/TextureMaterial.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

// choix de la sc?ne
// 1, 2, 3, 4, 5, 6, 7, 0xDEADBEEF, 0xFACADE, 0xCAU, 0xF
#define _CURRENT_TP 0xDEADBEEF

#define DATA_PATH "obj/"

std::vector<float> RT_ISICG::Rand::diss;

namespace RT_ISICG
{
	void Scene::tp1( Vec3f & cameraPosition, Vec3f & cameraLookAt )
	{
		// Camera position & orientation
		cameraPosition = Vec3f( 0.f, 0.f, -2.f );
		cameraLookAt   = Vec3f( 0.f, 0.f, 79.f );

		// Add objects.
		_addObject( new Sphere( "Sphere", Vec3f( 0.f, 0.f, 3.5f ), 1.f ) );

		// Add materials.
		_addMaterial( new ColorMaterial( "Blue", BLUE ) );

		// Link objects and materials.
		_attachMaterialToObject( "Blue", "Sphere" );

		// Add lighting.
		_addLight( new PointLight( cameraPosition, WHITE, 20.f ) );
	}

	void Scene::tp2( Vec3f & cameraPosition, Vec3f & cameraLookAt )
	{
		// Add objects.
		_addObject( new Sphere( "Sphere", Vec3f( 0.f, 0.f, 8.f ), 1.f ) );
		_addObject( new Plane( "Plane", Vec3f( 0.f, -2.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );

		// Add materials.
		_addMaterial( new ColorMaterial( "Blue", BLUE ) );
		_addMaterial( new ColorMaterial( "Red", RED ) );

		// Link objects and materials.
		_attachMaterialToObject( "Blue", "Sphere" );
		_attachMaterialToObject( "Red", "Plane" );

		// Add lighting.
		_addLight( new PointLight( Vec3f( 1.f, 10.f, 1.f ), WHITE, 100.f ) );
	}

	void Scene::tp3( Vec3f & cameraPosition, Vec3f & cameraLookAt )
	{
		// Add objects.
		_addObject( new Sphere( "Sphere", Vec3f( 0.f, 0.f, 5.f ), 1.f ) );
		_addObject( new Plane( "Plane", Vec3f( 0.f, -2.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );

		// Add materials.
		_addMaterial( new PlasticMaterial( "plastic", GREY, GREY ) );

		// Link objects and materials.
		_attachMaterialToObject( "plastic", "Sphere" );
		_attachMaterialToObject( "plastic", "Plane" );

		// Add lighting.
		// quader-shaped light-source
		_addLight(
			new QuadLight( Vec3f( 1.f, 10.f, 2.f ), Vec3f( -2.f, 0.f, 0.f ), Vec3f( 0.f, 0.f, 2.f ), WHITE, 40.f ) );

		// triangle-shaped light-source
		//_addLight(
		//	new TriLight( Vec3f( 1.f, 10.f, 2.f ), Vec3f( 3.f, 1.f, 2.f ), Vec3f( 1.f, 1.f, 4.f ), WHITE, 30.f ) );

		// sphere-shaped light-source
		//_addLight( new GlobeLight( Vec3f( -2.f, 4.f, 3.f ), 1.f, WHITE, 6.f ) );
	}

	void Scene::tp4( Vec3f & cameraPosition, Vec3f & cameraLookAt )
	{
		// Add objects.
		_addObject( new Sphere( "SphereMatte", Vec3f( 1.5f, 0.f, 3.f ), 0.5f ) );
		_addObject( new Sphere( "SphereLambert", Vec3f( 0.5f, 0.f, 3.f ), 0.5f ) );
		_addObject( new Sphere( "SpherePlastic", Vec3f( -0.5f, 0.f, 3.f ), 0.5f ) );
		_addObject( new Sphere( "SphereRealistic", Vec3f( -1.5f, 0.f, 3.f ), 0.5f ) );
		_addObject( new Plane( "Plane", Vec3f( 0.f, -2.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );

		// Add materials.
		_addMaterial( new LambertMaterial( "lambert", GREY ) );
		_addMaterial( new PlasticMaterial( "plastic", GREY, GREY ) );
		_addMaterial( new MatteMaterial( "matte", GREY, 0.6f ) );
		_addMaterial( new RealisticMaterial( "realistic", GREY, GREY, 0.5f ) );

		// Link objects and materials.
		_attachMaterialToObject( "lambert", "Plane" );
		_attachMaterialToObject( "lambert", "SphereLambert" );
		_attachMaterialToObject( "plastic", "SpherePlastic" );
		_attachMaterialToObject( "matte", "SphereMatte" );
		_attachMaterialToObject( "realistic", "SphereRealistic" );

		// Add lighting
		_addLight( new PointLight( Vec3f( 0.f, 0.f, -2.f ), WHITE, 60.f ) );
	}

	void Scene::tp5( Vec3f & cameraPosition, Vec3f & cameraLookAt )
	{
		// Camera position & orientation
		cameraPosition = Vec3f( 0.f, 2.f, -6.f );
		cameraLookAt   = Vec3f( 0.f, 2.f, 0.f );

		// ================================================================
		// Add materials.
		// ================================================================
		_addMaterial( new MatteMaterial( "WhiteMatte", WHITE, 0.6f ) );
		_addMaterial( new MatteMaterial( "RedMatte", RED, 0.6f ) );
		_addMaterial( new MatteMaterial( "GreenMatte", GREEN, 0.6f ) );
		_addMaterial( new MatteMaterial( "BlueMatte", BLUE, 0.6f ) );
		_addMaterial( new MatteMaterial( "GreyMatte", GREY, 0.6f ) );
		_addMaterial( new MatteMaterial( "MagentaMatte", MAGENTA, 0.6f ) );

		_addMaterial( new MirrorMaterial( "WhiteMirror" ) );
		_addMaterial( new TransparentMaterial( "WhiteTransparent", 1.3f ) );

		// ================================================================
		// Add objects.
		// ================================================================
		// Spheres .
		_addObject( new Sphere( "Sphere1", Vec3f( -2.f, 0.f, 3.f ), 1.5f ) );
		_attachMaterialToObject( "WhiteMirror", "Sphere1" );
		_addObject( new Sphere( "Sphere2", Vec3f( 2.f, 0.f, 3.f ), 1.5f ) );
		_attachMaterialToObject( "WhiteTransparent", "Sphere2" );
		// Pseudo Cornell box made with infinite planes .
		_addObject( new Plane( "PlaneGround", Vec3f( 0.f, -3.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );
		_attachMaterialToObject( "GreyMatte", "PlaneGround" );
		_addObject( new Plane( "PlaneLeft", Vec3f( 5.f, 0.f, 0.f ), Vec3f( -1.f, 0.f, 0.f ) ) );
		_attachMaterialToObject( "RedMatte", "PlaneLeft" );
		_addObject( new Plane( "PlaneCeiling", Vec3f( 0.f, 7.f, 0.f ), Vec3f( 0.f, -1.f, 0.f ) ) );
		_attachMaterialToObject( "GreenMatte", "PlaneCeiling" );
		_addObject( new Plane( "PlaneRight", Vec3f( -5.f, 0.f, 0.f ), Vec3f( 1.f, 0.f, 0.f ) ) );
		_attachMaterialToObject( "BlueMatte", "PlaneRight" );
		_addObject( new Plane( "PlaneFront", Vec3f( 0.f, 0.f, 10.f ), Vec3f( 0.f, 0.f, -1.f ) ) );
		_attachMaterialToObject( "MagentaMatte", "PlaneFront" );

		// ================================================================
		// Add lights.
		// ================================================================
		_addLight( new PointLight( Vec3f( 0.f, 5.f, 0.f ), WHITE, 100.f ) );
	}

	void Scene::tp6( Vec3f & cameraPosition, Vec3f & cameraLookAt )
	{
#define CONF 0
#if CONF
		cameraPosition = Vec3f( -250.f, 500.f, 330.f );
		cameraLookAt   = Vec3f( 0.f, 350.f, 100.f );
		loadFileTriangleMesh( "conference", DATA_PATH + std::string( "conference/" ), "conference.obj" );
		_addLight(
			new QuadLight( Vec3f( 900, 600, -300 ), Vec3f( -800.f, 0.f, 0.f ), Vec3f( 0.f, 0.f, 300.f ), WHITE, 20.f ) );
#else
		_addMaterial( new PlasticMaterial( "plastic", WHITE, WHITE ) );
		cameraPosition = Vec3f( 0.f, 0.f, -6.f );
		cameraLookAt   = Vec3f( 0.f, 0.f, 0.f );
		loadFileTriangleMesh( "bunny", DATA_PATH, "bunny.obj" );
		_attachMaterialToObject( "plastic", "bunny" );
		_addLight( new PointLight( Vec3f( 0.f, 2.f, -6.f ), WHITE, 60.f ) );
#endif
	}

	void Scene::tp7( Vec3f & cameraPosition, Vec3f & cameraLookAt ) {
		cameraPosition = Vec3f( 0.f, 0.f, 0.f );
		cameraLookAt   = Vec3f( 0.f, 0.f, 1.f );

		// materials
		_addMaterial( new PlasticMaterial( "RedPlastic", RED, RED ) );
		_addMaterial( new PlasticMaterial( "GreenPlastic", GREEN, GREEN ) );
		_addMaterial( new PlasticMaterial( "GreyPlastic", GREY / 2.f, GREY / 2.f ) );
		_addMaterial( new MirrorMaterial( "WhiteMirror" ) );

		// Pseudo Cornell box made with infinite planes .
		_addObject( new Plane( "PlaneGround", Vec3f( 0.f, -3.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );
		_attachMaterialToObject( "GreenPlastic", "PlaneGround" );
		_addObject( new Plane( "PlaneLeft", Vec3f( 5.f, 0.f, 0.f ), Vec3f( -1.f, 0.f, 0.f ) ) );
		_attachMaterialToObject( "RedPlastic", "PlaneLeft" );
		_addObject( new Plane( "PlaneCeiling", Vec3f( 0.f, 8.f, 0.f ), Vec3f( 0.f, -1.f, 0.f ) ) );
		_attachMaterialToObject( "GreenPlastic", "PlaneCeiling" );
		_addObject( new Plane( "PlaneRight", Vec3f( -5.f, 0.f, 0.f ), Vec3f( 1.f, 0.f, 0.f ) ) );
		_attachMaterialToObject( "RedPlastic", "PlaneRight" );

		// implicit surfaces
		_addObject( new ImplicitPlane( "PlaneFront", Vec3f( 0.f, 0.f, 15.f ), Vec3f( 0.f, 0.f, -1.f ) ) );
		_attachMaterialToObject( "WhiteMirror", "PlaneFront" );
		_addObject( new ImplicitCuboid( "cuboid", Vec3f( 2.f, 0.f, 8.f ), Vec3f(1.f) ) );
		_attachMaterialToObject( "GreyPlastic", "cuboid" );
		_addObject( new ImplicitSphere( "sphere", Vec3f( -2.f, 0.f, 8.f ), 1.f ) );
		_attachMaterialToObject( "GreyPlastic", "sphere" );

		// lights
		//_addLight( new PointLight( cameraPosition, WHITE, 300.f ) );
		_addLight(
			new QuadLight( Vec3f( 5.f, -3.f, -1.f ), Vec3f( -10.f, 0.f, 0.f ), Vec3f( 0.f, 6.f, 0.f ), WHITE, 10.f ) );
	}

	void Scene::lenses(Vec3f& cameraPosition, Vec3f& cameraLookAt) {
		// Camera position & orientation
		cameraPosition = Vec3f( 0.f, 0.f, 0.f );
		cameraLookAt   = Vec3f( 0.f, 0.f, 1.f );

		_addMaterial( new TransparentMaterial( "WhiteTransparent", 1.3f, 100.f, WHITE ) );
		_addMaterial( new MirrorMaterial( "WhiteMirror" ) );
		_addMaterial( new PlasticMaterial( "WhitePlastic", WHITE, WHITE ) );
		_addMaterial( new PlasticMaterial( "RedPlastic", RED, RED ) );
		_addMaterial( new PlasticMaterial( "GreenPlastic", GREEN, GREEN ) );
		_addMaterial( new PlasticMaterial( "BluePlastic", BLUE, BLUE ) );
		_addMaterial( new PlasticMaterial( "GreyPlastic", GREY, GREY ) );
		_addMaterial( new PlasticMaterial( "MagentaPlastic", MAGENTA, MAGENTA ) );

		_addMaterial( new TransparentMaterial( "LensMaterial1", 1.3f, 100.f, WHITE ) );
		_addMaterial( new TransparentMaterial( "LensMaterial2", 0.4f, 100.f, WHITE ) );

		// Pseudo Cornell box made with infinite planes .
		_addObject( new Plane( "PlaneGround", Vec3f( 0.f, -3.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );
		_attachMaterialToObject( "GreenPlastic", "PlaneGround" );
		_addObject( new Plane( "PlaneLeft", Vec3f( 5.f, 0.f, 0.f ), Vec3f( -1.f, 0.f, 0.f ) ) );
		_attachMaterialToObject( "RedPlastic", "PlaneLeft" );
		_addObject( new Plane( "PlaneCeiling", Vec3f( 0.f, 8.f, 0.f ), Vec3f( 0.f, -1.f, 0.f ) ) );
		_attachMaterialToObject( "GreenPlastic", "PlaneCeiling" );
		_addObject( new Plane( "PlaneRight", Vec3f( -5.f, 0.f, 0.f ), Vec3f( 1.f, 0.f, 0.f ) ) );
		_attachMaterialToObject( "RedPlastic", "PlaneRight" );
		_addObject( new Plane( "PlaneFront", Vec3f( 0.f, 0.f, 20.f ), Vec3f( 0.f, 0.f, -1.f ) ) );
		_attachMaterialToObject( "GreenPlastic", "PlaneFront" );

		/*_addObject( new Sphere( "sph1", Vec3f( 0.5f, 1.f, 12.f ), 0.8f ) );
		_attachMaterialToObject( "BluePlastic", "sph1" );
		_addObject( new Sphere( "sph2", Vec3f( -1.f, -0.5f, 8.f ), 0.8f ) );
		_attachMaterialToObject( "BluePlastic", "sph2" );
		_addObject( new Sphere( "sph3", Vec3f( 1.f, 0.f, 2.f ), 0.8f ) );
		_attachMaterialToObject( "BluePlastic", "sph3" );*/

		std::vector<ImplicitSurface *> surfaces1;
		surfaces1.emplace_back( new ImplicitCuboid( "cube", Vec3f( 2.f, 0.f, 6.f ), Vec3f( 0.5f ) ) );
		surfaces1.emplace_back( new ImplicitSphere( "sphere", Vec3f( 2.f, 0.f, 6.f ), 0.5f ) );
		_addObject( new ImplicitInterpolation( "interp1", surfaces1, 0.15f ) );
		_attachMaterialToObject( "BluePlastic", "interp1" );

		std::vector<ImplicitSurface *> surfaces2;
		surfaces2.emplace_back( new ImplicitCuboid( "cube", Vec3f( -2.f, 0.f, 6.f ), Vec3f( 0.5f ) ) );
		surfaces2.emplace_back( new ImplicitSphere( "sphere", Vec3f( -2.f, 0.f, 6.f ), 0.5f ) );
		_addObject( new ImplicitInterpolation( "interp2", surfaces2, 0.5f ) );
		_attachMaterialToObject( "BluePlastic", "interp2" );

		std::vector<ImplicitSurface *> surfaces3;
		surfaces3.emplace_back( new ImplicitCuboid( "cube", Vec3f( 2.f, 2.f, 6.f ), Vec3f( 0.5f ) ) );
		surfaces3.emplace_back( new ImplicitSphere( "sphere", Vec3f( 2.f, 2.f, 6.f ), 0.5f ) );
		_addObject( new ImplicitInterpolation( "interp3", surfaces3, 0.75f ) );
		_attachMaterialToObject( "BluePlastic", "interp3" );

		std::vector<ImplicitSurface *> surfaces4;
		surfaces4.emplace_back( new ImplicitCuboid( "cube", Vec3f( -2.f, 2.f, 6.f ), Vec3f( 0.5f ) ) );
		surfaces4.emplace_back( new ImplicitSphere( "sphere", Vec3f( -2.f, 2.f, 6.f ), 0.5f ) );
		_addObject( new ImplicitInterpolation( "interp4", surfaces4, 0.95f ) );
		_attachMaterialToObject( "BluePlastic", "interp4" );

		/*std::vector<ImplicitSurface *> lens1Surfaces;
		lens1Surfaces.emplace_back( new ImplicitSphere( "_1", Vec3f( 0.f, 0.f, 2.99f ), 2.f ) );
		lens1Surfaces.emplace_back( new ImplicitCuboid( "_2", Vec3f( 0.f, 0.f, -1.f ), Vec3f( 1.f, 1.f, 2.f ) ) );
		std::vector<ImplicitSurface *> lens1Surfaces2;
		lens1Surfaces2.emplace_back( new ImplicitIntersection( "lens1", lens1Surfaces ) );
		lens1Surfaces2.emplace_back( new ImplicitCuboid( "_3", Vec3f( 0.1f, 0.f, 1.99f ), Vec3f( 0.1f, 0.1f, 3.f ) ) );
		_addObject( new ImplicitIntersection( "lens1", lens1Surfaces ) );
		_attachMaterialToObject( "LensMaterial1", "lens1" );*/

		/*std::vector<ImplicitSurface *> lens2Surfaces;
		lens2Surfaces.emplace_back( new ImplicitSphere( "_1", Vec3f( 0.f, 0.f, 1.f ), 1.f ) );
		lens2Surfaces.emplace_back( new ImplicitCuboid( "_2", Vec3f( 0.f, 0.f, 2.99f ), Vec3f( 1.f, 1.f, 1.f ) ) );
		_addObject( new ImplicitIntersection( "lens2", lens2Surfaces ) );
		_attachMaterialToObject( "LensMaterial2", "lens2" );*/
		//_addLight( new PointLight( cameraPosition, WHITE, 200.f ) );
		_addLight( new PointLight( Vec3f( -2.9f, 7.f, 0.f ), WHITE, 200.f ) );
		_addLight( new PointLight( Vec3f( 2.9f, -2.9f, 0.f ), WHITE, 200.f ) );
	}

	void Scene::tp0( Vec3f & cameraPosition, Vec3f & cameraLookAt )
	{
		// Camera position & orientation
		cameraPosition = Vec3f( 0.f, 4.f, -6.f );
		cameraLookAt   = Vec3f( 0.f, 4.f, 1.f );

		// ================================================================
		// Add materials.
		// ================================================================
		_addMaterial( new PlasticMaterial( "WhitePlastic", WHITE, WHITE ) );
		_addMaterial( new PlasticMaterial( "RedPlastic", RED, RED ) );
		_addMaterial( new PlasticMaterial( "GreenPlastic", GREEN, GREEN ) );
		_addMaterial( new PlasticMaterial( "BluePlastic", BLUE, BLUE ) );
		_addMaterial( new PlasticMaterial( "GreyPlastic", GREY, GREY ) );
		_addMaterial( new PlasticMaterial( "MagentaPlastic", MAGENTA, MAGENTA ) );
		_addMaterial( new MirrorMaterial( "WhiteMirror" ) );
		_addMaterial( new TransparentMaterial( "SemiTransparent", 1.3f, 60.f, YELLOW ) );
		_addMaterial( new TransparentMaterial( "WhiteTransparent", 1.3f, 100.f, WHITE ) );

		// ================================================================
		// Add objects.
		// ================================================================
		// Spheres .
		_addObject( new Sphere( "bauch", Vec3f( 0.f, 0.5f, 8.f ), 3.f ) );
		_attachMaterialToObject( "BluePlastic", "bauch" );
		_addObject( new Sphere( "kopf", Vec3f( 0.f, 5.f, 8.f ), 1.5f ) );
		_attachMaterialToObject( "WhiteMirror", "kopf" );
		_addObject( new Sphere( "arm1", Vec3f( 3.f, 2.f, 8.f ), 0.75f ) );
		_attachMaterialToObject( "WhiteTransparent", "arm1" );
		_addObject( new Sphere( "arm2", Vec3f( -3.f, 2.f, 8.f ), 0.75f ) );
		_attachMaterialToObject( "WhiteTransparent", "arm2" );
		_addObject( new Sphere( "bein1", Vec3f( 2.f, -2.f, 8.f ), 0.75f ) );
		_attachMaterialToObject( "MagentaPlastic", "bein1" );
		_addObject( new Sphere( "bein2", Vec3f( -2.f, -2.f, 8.f ), 0.75f ) );
		_attachMaterialToObject( "MagentaPlastic", "bein2" );
		_addObject( new Sphere( "nase", Vec3f( 0.f, 5.f, 6.25f ), 0.25f ) );
		_attachMaterialToObject( "MagentaPlastic", "nase" );
		_addObject( new Sphere( "auge1", Vec3f( 0.75f, 5.25f, 6.75f ), 0.25f ) );
		_attachMaterialToObject( "RedPlastic", "auge1" );
		_addObject( new Sphere( "auge2", Vec3f( -0.75f, 5.25f, 6.75f ), 0.25f ) );
		_attachMaterialToObject( "RedPlastic", "auge2" );
		_addObject( new Sphere( "ohr1", Vec3f( -1.f, 6.f, 8.f ), 0.75f ) );
		_attachMaterialToObject( "MagentaPlastic", "ohr1" );
		_addObject( new Sphere( "ohr2", Vec3f( 1.f, 6.f, 8.f ), 0.75f ) );
		_attachMaterialToObject( "MagentaPlastic", "ohr2" );
		
		// Pseudo Cornell box made with infinite planes .
		_addObject( new Plane( "PlaneGround", Vec3f( 0.f, -3.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );
		_attachMaterialToObject( "GreenPlastic", "PlaneGround" );
		_addObject( new Plane( "PlaneLeft", Vec3f( 5.f, 0.f, 0.f ), Vec3f( -1.f, 0.f, 0.f ) ) );
		_attachMaterialToObject( "RedPlastic", "PlaneLeft" );
		_addObject( new Plane( "PlaneCeiling", Vec3f( 0.f, 8.f, 0.f ), Vec3f( 0.f, -1.f, 0.f ) ) );
		_attachMaterialToObject( "GreenPlastic", "PlaneCeiling" );
		_addObject( new Plane( "PlaneRight", Vec3f( -5.f, 0.f, 0.f ), Vec3f( 1.f, 0.f, 0.f ) ) );
		_attachMaterialToObject( "RedPlastic", "PlaneRight" );
		_addObject( new Plane( "PlaneFront", Vec3f( 0.f, 0.f, 10.f ), Vec3f( 0.f, 0.f, -1.f ) ) );
		_attachMaterialToObject( "GreenPlastic", "PlaneFront" );

		#define BIND_TRANSPARENT_MTL 1
		loadFileTriangleMesh( "Pool", DATA_PATH, "mar.obj" );

		// ================================================================
		// Add lights.
		// ================================================================
		//_addGlobeLightObject( "lo", Vec3f( 2.f, 4.f, 2.f ), 0.6f, WHITE, BLUE, 20.f );
		_addLight( new PointLight( Vec3f( -2.9f, 7.f, 0.f ), WHITE, 100.f ) );
		_addLight( new PointLight( Vec3f( 2.9f, -2.9f, 0.f ), WHITE, 100.f ) );
	}

	void Scene::caustics( Vec3f & cameraPosition, Vec3f & cameraLookAt )
	{
		cameraPosition = Vec3f( 0.f, 5.f, -5.f );
		cameraLookAt   = Vec3f( 0.f, 1.f, 0.f );
		_addMaterial( new TransparentMaterial( "WhiteTransparent", 1.3f, 100.f, WHITE ) );
		_addMaterial( new MirrorMaterial( "WhiteMirror" ) );
		_addMaterial( new PlasticMaterial( "BluePlastic", BLUE, BLUE ) );
		_addMaterial( new PlasticMaterial( "RedPlastic", RED, RED ) );
		_addMaterial( new PlasticMaterial( "GreenPlastic", GREEN, GREEN ) );
		_addMaterial( new PlasticMaterial( "WhitePlastic", WHITE, WHITE ) );

		_addObject( new Sphere( "sphere1", Vec3f( -2.f, -2.f, 5.f ), 1.f ) );
		_attachMaterialToObject( "WhiteTransparent", "sphere1" );
		_addObject( new Sphere( "sphere2", Vec3f( -1.f, -2.f, 4.f ), 1.f ) );
		_attachMaterialToObject( "WhiteTransparent", "sphere2" );

		_addObject( new Plane( "PlaneGround", Vec3f( 0.f, -3.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );
		_attachMaterialToObject( "GreenPlastic", "PlaneGround" );
		_addObject( new Plane( "PlaneLeft", Vec3f( 5.f, 0.f, 0.f ), Vec3f( -1.f, 0.f, 0.f ) ) );
		_attachMaterialToObject( "RedPlastic", "PlaneLeft" );
		_addObject( new Plane( "PlaneCeiling", Vec3f( 0.f, 8.f, 0.f ), Vec3f( 0.f, -1.f, 0.f ) ) );
		_attachMaterialToObject( "GreenPlastic", "PlaneCeiling" );
		_addObject( new Plane( "PlaneRight", Vec3f( -5.f, 0.f, 0.f ), Vec3f( 1.f, 0.f, 0.f ) ) );
		_attachMaterialToObject( "RedPlastic", "PlaneRight" );
		_addObject( new Plane( "PlaneFront", Vec3f( 0.f, 0.f, 10.f ), Vec3f( 0.f, 0.f, -1.f ) ) );
		_attachMaterialToObject( "GreenPlastic", "PlaneFront" );

		_addLight( new PointLight( Vec3f( 0.f, -1.f, 6.f ), WHITE, 50.f ) );
	}

	void Scene::textures( Vec3f & cameraPosition, Vec3f & cameraLookAt )
	{
		cameraPosition = Vec3f( -700.f, 160.f, 0.f );
		cameraLookAt   = Vec3f( -690.f, 158.f, 0.f );
		loadFileTriangleMesh( "test", DATA_PATH + std::string("test/"), "test.obj" );
		//_addLight( new PointLight( Vec3f( -700.f, 80.f, 0.f ), WHITE, 1000000.f ) );
		_addLight( new QuadLight( Vec3f( 800.f, 1000.f, -600.f ),
								  Vec3f( -1800.f, 1000.f, 0.f ),
								  Vec3f( 0.f, 1000.f, 1200.f ),
								  Vec3f( 225.f, 157.f, 55.f ),
								  0.2f ) );
	}

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

	void Scene::init( Vec3f & cameraPosition, Vec3f & cameraLookAt )
	{
		generateRandoms();

#if _CURRENT_TP == 0xDEADBEEF
		tp0( cameraPosition, cameraLookAt );
#elif _CURRENT_TP == 1
		tp1( cameraPosition, cameraLookAt );
#elif _CURRENT_TP == 2
		tp2( cameraPosition, cameraLookAt );
#elif _CURRENT_TP == 3
		tp3( cameraPosition, cameraLookAt );
#elif _CURRENT_TP == 4
		tp4( cameraPosition, cameraLookAt );
#elif _CURRENT_TP == 5
		tp5( cameraPosition, cameraLookAt );
#elif _CURRENT_TP == 6
		tp6( cameraPosition, cameraLookAt );
#elif _CURRENT_TP == 7
		tp7( cameraPosition, cameraLookAt );
#elif _CURRENT_TP == 0xFACADE
		lenses( cameraPosition, cameraLookAt );
#elif _CURRENT_TP == 0xCAU
		caustics( cameraPosition, cameraLookAt );
#elif _CURRENT_TP == 0xF
		textures( cameraPosition, cameraLookAt );
#endif
	}

	void Scene::loadFileTriangleMesh( const std::string & p_name, const std::string & p_filePath, const std::string & p_objPath )
	{
		std::cout << "Loading: " << p_filePath + p_objPath << std::endl;
		Assimp::Importer importer;

		// Read scene and triangulate meshes
		const aiScene * const scene
			= importer.ReadFile( p_filePath + p_objPath, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords );

		if ( scene == nullptr ) { throw std::runtime_error( "Fail to load file: " + p_filePath + p_objPath ); }

		unsigned int cptTriangles = 0;
		unsigned int cptVertices  = 0;

		for ( unsigned int m = 0; m < scene->mNumMeshes; ++m )
		{
			const aiMesh * const mesh = scene->mMeshes[ m ];
			if ( mesh == nullptr )
			{
				throw std::runtime_error( "Fail to load file: " + p_filePath + p_objPath + ": mesh is null" );
			}

			const std::string meshName = p_name + "_" + std::string( mesh->mName.C_Str() );
			std::cout << "-- Load mesh " << m + 1 << "/" << scene->mNumMeshes << ": " << meshName << std::endl;

			cptTriangles += mesh->mNumFaces;
			cptVertices += mesh->mNumVertices;

			const bool hasUV = mesh->HasTextureCoords( 0 );

			MeshTriangle * triMesh = new MeshTriangle( meshName );
			// Vertices before faces otherwise face normals cannot be computed.
			for ( unsigned int v = 0; v < mesh->mNumVertices; ++v )
			{
				triMesh->addVertex( mesh->mVertices[ v ].x, mesh->mVertices[ v ].y, mesh->mVertices[ v ].z );
				triMesh->addNormal( mesh->mNormals[ v ].x, mesh->mNormals[ v ].y, mesh->mNormals[ v ].z );
				if ( hasUV ) triMesh->addUV( mesh->mTextureCoords[ 0 ][ v ].x, mesh->mTextureCoords[ 0 ][ v ].y );
			}
			for ( unsigned int f = 0; f < mesh->mNumFaces; ++f )
			{
				const aiFace & face = mesh->mFaces[ f ];
				triMesh->addTriangle( face.mIndices[ 0 ], face.mIndices[ 1 ], face.mIndices[ 2 ] );
			}

			// build BVH for specific mesh
			triMesh->buildBVH();

			_addObject( triMesh );

			const aiMaterial * const mtl = scene->mMaterials[ mesh->mMaterialIndex ];
			if ( mtl == nullptr )
			{
				std::cerr << "Material undefined," << meshName << " assigned to default material" << std::endl;
			}
			else
			{
				Vec3f kd = WHITE;
				Vec3f ks = BLACK;
				float s	 = 0.f;
				float tr = 0.f;

				aiColor3D aiKd;
				if ( mtl->Get( AI_MATKEY_COLOR_DIFFUSE, aiKd ) == AI_SUCCESS ) kd = Vec3f( aiKd.r, aiKd.g, aiKd.b );
				aiColor3D aiKs;
				if ( mtl->Get( AI_MATKEY_COLOR_SPECULAR, aiKs ) == AI_SUCCESS ) ks = Vec3f( aiKs.r, aiKs.g, aiKs.b );
				float aiS = 0.f;
				if ( mtl->Get( AI_MATKEY_SHININESS, aiS ) == AI_SUCCESS ) s = aiS;
				float Tr = 0.f;
				if ( mtl->Get( AI_MATKEY_OPACITY, Tr ) == AI_SUCCESS ) tr = Tr;
				aiString mtlName;
				mtl->Get( AI_MATKEY_NAME, mtlName );

				aiTexture aiTex;
				aiString  texturePath;
				if ( tr == 0.f )
				{
					std::cout << "transparent" << std::endl;
					_addMaterial( new TransparentMaterial( "WhiteTransparent", 1.3f, 100.f, WHITE ) );
					_attachMaterialToObject( "WhiteTransparent", meshName );
				}
				else if ( mtl->GetTexture( aiTextureType_DIFFUSE, 0, &texturePath ) == AI_SUCCESS )
				{
					std::cout << "texture" << std::endl;
					std::string path = p_filePath + texturePath.C_Str();
					_addMaterial( new TextureMaterial( std::string( mtlName.C_Str() ), path ) );
					_attachMaterialToObject( mtlName.C_Str(), meshName );
				}
				else
				{
					std::cout << "else" << std::endl;
#if BIND_TRANSPARENT_MTL
					_addMaterial( new TransparentMaterial( "WhiteTransparent", 1.3f, 100.f, WHITE ) );
					_attachMaterialToObject( "WhiteTransparent", meshName );
#else
					_addMaterial( new PlasticMaterial( std::string( mtlName.C_Str() ), kd, ks ) );
					_attachMaterialToObject( mtlName.C_Str(), meshName );
#endif
				}
			}

			std::cout << "-- [DONE] " << triMesh->getNbTriangles() << " triangles, " << triMesh->getNbVertices()
					  << " vertices." << std::endl;
		}
		std::cout << "[DONE] " << scene->mNumMeshes << " meshes, " << cptTriangles << " triangles, " << cptVertices
				  << " vertices." << std::endl;
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

	void Scene::_addGlobeLightObject( const std::string & p_name,
									  const Vec3f &		  p_position,
									  const float		  p_radius,
									  const Vec3f &		  p_objectColor,
									  const Vec3f &		  p_lightColor,
									  const float		  p_power )
	{
		_addMaterial( new EmissiveMaterial( p_name + "mat", p_lightColor, p_objectColor, p_radius * 2.f ) );
		GlobeLight * light = new GlobeLight( p_position, p_radius, p_lightColor, p_power );
		_addLight( light );
		_addObject( new SphereLightObject( p_name, p_position, p_radius - 0.001f, p_objectColor, p_power, light ) );
		_attachMaterialToObject( p_name + "mat", p_name );
	}

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
