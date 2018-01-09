#pragma once
#include "OGLRenderer.h"

enum MeshBuffer {
	VERTEX_BUFFER, COLOUR_BUFFER, TEXTURE_BUFFER, NORMAL_BUFFER, TANGENT_BUFFER, INDEX_BUFFER, MAX_BUFFER
};
class Mesh
{
public:
	Mesh(void);
	~Mesh(void);

	virtual void Draw();
	static Mesh* GenerateTriangle(float w);
	static Mesh* GenerateQuad();
	static Mesh* GenerateCircle();
	static Mesh* GenerateCube();
	void SetTexture(GLuint tex) { texture = tex; }
	void SetBumpMap(GLuint tex) { bumpTexture = tex; }
	GLuint GetTexture() { return texture; }
	GLuint GetBumpMap() { return bumpTexture; }


protected:
	void GenerateNormals();
	void GenerateTangents();
	void BufferData();

	GLuint arrayObject;
	GLuint bufferObject[MAX_BUFFER];
	GLuint numVerticies;
	GLuint type;
	GLuint texture;
	GLuint numIndicies;
	GLuint bumpTexture;

	unsigned int* indicies;

	Vector2* textureCoords;
	Vector3* verticies;
	Vector3* normals;
	Vector3* tangents;
	Vector4* colours;

	Vector3 GenerateTangent(const Vector3 &a, const Vector3 &b,
							const Vector3 &c, const Vector2 & ta,
							const Vector2 & tb, const Vector2 & tc);
};

