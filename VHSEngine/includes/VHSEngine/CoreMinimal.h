#pragma once

#include <iostream>
#include <vector>

using namespace std;

typedef unsigned int vhsuint;

//class/type definitions
class GraphicsEngine;
typedef shared_ptr<GraphicsEngine> GraphicsEnginePtr;

class VertexArrayObject;
typedef VertexArrayObject VAO;
typedef shared_ptr<VAO> VAOPtr;


class Texture;
typedef shared_ptr<Texture> TexturePtr;
typedef vector<TexturePtr> TexturePtrStack;


class ShaderProgram;
typedef shared_ptr<ShaderProgram> ShaderPtr;

class Mesh;
typedef shared_ptr<Mesh> MeshPtr;
typedef vector<MeshPtr> MeshPtrStack;

class Model;
typedef shared_ptr<Model> ModelPtr;
typedef vector<ModelPtr> ModelPtrStack;

class Material;
typedef shared_ptr<Material> MaterialPtr;
typedef vector<MaterialPtr> MaterialPtrStack;

class Camera;
typedef shared_ptr<Camera> CameraPtr;

class Collision;
typedef shared_ptr<Collision> CollisionPtr;
typedef vector<CollisionPtr> CollisionStack;

class BoxCollision;
typedef shared_ptr<BoxCollision> BoxCollisionPtr;

//useful for shape data and VAOs Matrices
typedef vector<float> PositionMatrix;
typedef vector<vhsuint> IndicesMatrix;

//this structure will handle matrices for VAOs
struct ShapeData {
	vector<float> PositionMatrix;
	vector<vhsuint> IndicesMatrix;
};


//a list of the geometric shapes available in VAO
enum class GeometricShapes {
	Triangle = 0,
	Polygon, 
	Circle,
	Cube

};

//store the parameter required to create a shader program
struct VFShaderParams {
	const wchar_t* VertexShaderPath;
	const wchar_t* FragmentShaderPath;
};

//the types of shader that can be accepted in out shader program
enum class ShaderTypes {
	Vertex = 0,
	Fragment
};