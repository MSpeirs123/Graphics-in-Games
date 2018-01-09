#pragma once

#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/HeightMap.h"
#include "../nclgl/TextMesh.h"
#include "../nclgl/ParticleEmitter.h"
#include "../nclgl/OBJMesh.h"
#include "../nclgl/MD5Mesh.h"

#define POST_PASSES 10
#define SHADOWSIZE 2048 

class Renderer_Scene : public OGLRenderer {
public:
	Renderer_Scene(Window & parent);
	virtual ~Renderer_Scene(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);

	void DrawUfo();
	void DrawLaser();

	void	DrawText(const std::string &text, const Vector3 &position, const float size = 10.0f, const bool perspective = false);

protected:
	void PresentScene();
	void DrawPostProcess();
	void DrawHeightmap();
	void DrawSun();
	void DrawWater();
	void DrawRoad();
	void DrawSkybox();
	void DrawBuilding1(int i);
	void DrawBuilding2(int i);
	void DrawBuilding3(int i);
	void DrawBuilding4(int i);
	void Scenefade(float w);
	void DrawEmitter();
	void SetShaderParticleSize(float f);
	void DrawHellKnight(Vector3 pos);
	void DrawShadowScene();
	void DrawCombinedScene();

	Shader* lightShader;
	Shader* reflectShader;
	Shader* sunShader;
	Shader* skyboxShader;
	Shader* textShader;
	Shader* fadeShader;
	Shader* particleShader;
	Shader* processShader;
	Shader* hellShader;
	Shader* shadowShader;
	Shader* sceneShader;

	Font*	basicFont;
	HeightMap* heightMap;
	Mesh* sun;
	Mesh* quad;
	Mesh* pproc;
	Mesh* road;
	Mesh* ufo;
	Mesh* laser;
	Mesh* building1;
	Mesh* building2;
	Mesh* building3;
	Mesh* building4;

	Light* light;
	Camera* camera;
	ParticleEmitter* emitter;
	OBJMesh* sphere;
	Light* pointLights;
	MD5FileData* hellData;
	MD5Node* hellNode;

	GLuint cubeMap;
	GLuint bufferFBO;
	GLuint processFBO;
	GLuint bufferColourTex[2];
	GLuint bufferDepthTex;
	GLuint shadowTex;
	GLuint shadowFBO;

	Vector3 ufopos = Vector3(10000.0f, 3000.0f,
		2000.0f);
	Vector3 hkpos = Vector3(3000, 350, 2000);
	float ptch = -60;

	float waterRotate;
	float laserRotate;
	int fps;
	Matrix4 shadowViewProj;
};