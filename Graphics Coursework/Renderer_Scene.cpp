#include "Renderer_Scene.h"
#include <math.h>
#include "../../nclgl/MD5Node.h"

int scenenom = 0;
float time;
float w1;
bool pause = false;

Renderer_Scene::Renderer_Scene(Window & parent) : OGLRenderer(parent)
{
	camera = new Camera();
	heightMap = new HeightMap("../../Textures/terrain.raw");
	sun = Mesh::GenerateCircle();
	quad = Mesh::GenerateQuad();
	pproc = Mesh::GenerateQuad();
	road = Mesh::GenerateQuad();
	building1 = Mesh::GenerateCube();
	building2 = Mesh::GenerateCube();
	building3 = Mesh::GenerateCube();
	building4 = Mesh::GenerateCube();
	ufo = Mesh::GenerateCircle();
	laser = Mesh::GenerateCube();
	basicFont = new Font(SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);
	camera->SetPosition(Vector3(RAW_WIDTH * HEIGHTMAP_X / 2.0f + 700,
		500.0f, RAW_WIDTH * HEIGHTMAP_X));
	camera->SetPitch(-60);

	emitter = new ParticleEmitter();

	light = new Light(Vector3(15000.0f, 3000.0f,
		5000.0f),
		Vector4(0.9f, 0.9f, 1.0f, 1),
		(RAW_WIDTH * HEIGHTMAP_X) / 0.000001f);

	reflectShader = new Shader("../../Shaders/PerPixelVertex.glsl",
		"../../Shaders/reflectFragment.glsl");
	skyboxShader = new Shader("../../Shaders/skyboxVertex.glsl",
		"../../Shaders/skyboxFragment.glsl");
	lightShader = new Shader("../../Shaders/bumpvertex.glsl",
		"../../Shaders/bumpfragment.glsl");
	sunShader = new Shader(SHADERDIR"SceneVertex.glsl",
		SHADERDIR"SceneFragment.glsl");
	sceneShader = new Shader(SHADERDIR"shadowscenevert.glsl",
		SHADERDIR"shadowscenefrag.glsl");
	shadowShader = new Shader(SHADERDIR"shadowVert.glsl", SHADERDIR"shadowFrag.glsl");
	textShader = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl");
	fadeShader = new Shader(SHADERDIR"basicVertex.glsl", SHADERDIR"colourFragment.glsl");
	particleShader = new Shader(SHADERDIR"vertex.glsl", SHADERDIR"fragment.glsl", SHADERDIR"geometry.glsl");
	processShader = new Shader("../../Shaders/TexturedVertex.glsl", SHADERDIR"tempbloom.glsl");
	hellShader = new Shader(SHADERDIR"skeletonVertex.glsl", SHADERDIR"skeletonFrag.glsl");

	if (!reflectShader->LinkProgram() || !lightShader->LinkProgram() ||
		!skyboxShader->LinkProgram() || !textShader->LinkProgram() ||
		!sunShader->LinkProgram() || !fadeShader->LinkProgram() ||
		!particleShader->LinkProgram() || !processShader->LinkProgram() ||
		!hellShader->LinkProgram() || !sceneShader->LinkProgram() ||
		!shadowShader->LinkProgram()) {
		return;
	}

	hellData = new MD5FileData(MESHDIR"hellknight.md5mesh");
	hellNode = new MD5Node(*hellData);

	hellData->AddAnim(MESHDIR"walk7.md5anim");
	hellNode->PlayAnim(MESHDIR"walk7.md5anim");

	quad->SetTexture(SOIL_load_OGL_texture("../../Textures/water512.jpg",	//http://vterrain.org/Water/water512.jpg
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	sun->SetTexture(SOIL_load_OGL_texture("../../Textures/sun.jpg",			//https://i.ytimg.com/vi/nUWfZfsW7uU/maxresdefault.jpg
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	road->SetTexture(SOIL_load_OGL_texture("../../Textures/road.jpg",		//http://4.bp.blogspot.com/-z4u0XXGeIw4/U7uxbHswANI/AAAAAAAAFcw/dN-s_WeTa5w/s1600/(ASPHALT+1)+seamless+tarmac+road+tar+texture.jpg
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	road->SetBumpMap(SOIL_load_OGL_texture(
		"../../Textures/Barren RedsDOT3.JPG", SOIL_LOAD_AUTO,				
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	ufo->SetTexture(SOIL_load_OGL_texture("../../Textures/alien.jpg",		//https://www.filterforge.com/filters/6220.jpg
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	ufo->SetBumpMap(SOIL_load_OGL_texture(
		"../../Textures/Barren RedsDOT3.JPG", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	laser->SetTexture(SOIL_load_OGL_texture("../../Textures/laser.png",		//http://mysticfirelights.com/wp-content/uploads/2016/03/starrynightLEDcombo-300x291.png
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	laser->SetBumpMap(SOIL_load_OGL_texture(
		"../../Textures/Barren RedsDOT3.JPG", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	heightMap->SetTexture(SOIL_load_OGL_texture(
		"../../Textures/Barren_Reds.JPG", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	heightMap->SetBumpMap(SOIL_load_OGL_texture(
		"../../Textures/Barren RedsDOT3.JPG", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	building1->SetTexture(SOIL_load_OGL_texture("../../Textures/skyscrap1.jpg",		//https://www.sketchuptextureclub.com/public/texture_d/0066-building-skyscraper-texture-seamless-hr.jpg
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	building1->SetBumpMap(SOIL_load_OGL_texture(
		"../../Textures/brickDOT3.tga", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	building2->SetTexture(SOIL_load_OGL_texture("../../Textures/skyscrap2.jpg",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	building2->SetBumpMap(SOIL_load_OGL_texture(
		"../../Textures/brickDOT3.tga", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	building3->SetTexture(SOIL_load_OGL_texture("../../Textures/skyscrap3.jpg",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	building3->SetBumpMap(SOIL_load_OGL_texture(
		"../../Textures/brickDOT3.tga", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	building4->SetTexture(SOIL_load_OGL_texture("../../Textures/skyscrap4.jpg",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	building4->SetBumpMap(SOIL_load_OGL_texture(
		"../../Textures/brickDOT3.tga", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	emitter->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"laser.png",		//http://mysticfirelights.com/wp-content/uploads/2016/03/starrynightLEDcombo-300x291.png
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT));

	cubeMap = SOIL_load_OGL_cubemap(
		"../../Textures/cubemap (1)/cubemap/px.png", "../../Textures/cubemap (1)/cubemap/nx.png",
		"../../Textures/cubemap (1)/cubemap/py.png", "../../Textures/cubemap (1)/cubemap/ny.png",
		"../../Textures/cubemap (1)/cubemap/pz.png", "../../Textures/cubemap (1)/cubemap/nz.png",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID, 0
	);

	if (!cubeMap)
		return;
	if (!quad->GetTexture())
		return;
	if (!sun->GetTexture())
		return;
	if (!road->GetTexture() || !road->GetBumpMap())
		return;
	if (!ufo->GetTexture() || !ufo->GetBumpMap())
		return;
	if (!laser->GetTexture() || !laser->GetBumpMap())
		return;
	if (!heightMap->GetTexture())
		return;
	if (!building1->GetTexture() || !building1->GetBumpMap())
		return;
	if (!building2->GetTexture() || !building2->GetBumpMap())
		return;
	if (!building3->GetTexture() || !building3->GetBumpMap())
		return;
	if (!building4->GetTexture() || !building4->GetBumpMap())
		return;
	if (!emitter->GetTexture())
		return;
	if (!cubeMap || !quad->GetTexture() || !heightMap->GetTexture() ||
		!heightMap->GetBumpMap()) {
		return;
	}

	SetTextureRepeating(quad->GetTexture(), true);
	SetTextureRepeating(sun->GetTexture(), true);
	SetTextureRepeating(road->GetTexture(), true);
	SetTextureRepeating(road->GetBumpMap(), true);
	SetTextureRepeating(ufo->GetTexture(), true);
	SetTextureRepeating(ufo->GetBumpMap(), true);
	SetTextureRepeating(laser->GetTexture(), true);
	SetTextureRepeating(laser->GetBumpMap(), true);
	SetTextureRepeating(heightMap->GetTexture(), true);
	SetTextureRepeating(heightMap->GetBumpMap(), true);
	SetTextureRepeating(building1->GetTexture(), true);
	SetTextureRepeating(building1->GetBumpMap(), true);
	SetTextureRepeating(building2->GetTexture(), true);
	SetTextureRepeating(building2->GetBumpMap(), true);
	SetTextureRepeating(building3->GetTexture(), true);
	SetTextureRepeating(building3->GetBumpMap(), true);
	SetTextureRepeating(building4->GetTexture(), true);
	SetTextureRepeating(building4->GetBumpMap(), true);

	init = true;
	waterRotate = 0.0f;
	laserRotate = 0.0f;

	projMatrix = Matrix4::Perspective(1.0f, 15000.0f,
		(float)width / (float)height, 45.0f);

	//post processing FBO

	glGenTextures(1, &bufferDepthTex);
	glBindTexture(GL_TEXTURE_2D, bufferDepthTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height,
		0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	
	for (int i = 0; i < 2; ++i) {
		glGenTextures(1, &bufferColourTex[i]);
		glBindTexture(GL_TEXTURE_2D, bufferColourTex[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	}
	glGenFramebuffers(1, &bufferFBO); 
	glGenFramebuffers(1, &processFBO); 

	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, bufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
		GL_TEXTURE_2D, bufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, bufferColourTex[0], 0);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
		GL_FRAMEBUFFER_COMPLETE || !bufferDepthTex || !bufferColourTex[0]) {
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//shadow FBO
	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
		GL_COMPARE_R_TO_TEXTURE);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &shadowFBO);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//Various other GL code
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}



Renderer_Scene::~Renderer_Scene(void)
{
	delete camera;
	delete heightMap;
	delete sun;
	delete quad;
	delete pproc;
	delete road;
	delete ufo;
	delete laser;
	delete building1;
	delete building2;
	delete building3;
	delete building4;
	delete light;
	delete emitter;
	delete hellData;
	delete hellNode;

	delete reflectShader;
	delete skyboxShader;
	delete lightShader;
	delete sunShader;
	delete fadeShader;
	delete textShader;
	delete particleShader;
	delete processShader;
	delete shadowShader;
	delete sceneShader;

	currentShader = 0;

	glDeleteTextures(2, bufferColourTex);
	glDeleteTextures(1, &bufferDepthTex);
	glDeleteFramebuffers(1, &bufferFBO);
	glDeleteFramebuffers(1, &processFBO);
	glDeleteTextures(1, &shadowTex);
	glDeleteFramebuffers(1, &shadowFBO);
}

void Renderer_Scene::UpdateScene(float msec) {
	emitter->Update(msec);
	camera->UpdateCamera(msec);
	hellNode->Update(msec);
	if (hellNode->get_frame() == 37) {
		hkpos.x = hkpos.x + 120;
	}
	hellNode->SetTransform(hellNode->GetTransform() * Matrix4::Translation(hkpos));
	viewMatrix = camera->BuildViewMatrix();
	waterRotate += msec / 1000.0f;
	laserRotate += msec / 10.0f;
	fps = 1000 / msec;
	++time;
}

void Renderer_Scene::RenderScene() {
	float trs = hkpos.x - 100;
	Vector3 trsvec = Vector3(trs, hkpos.y, hkpos.z);
	string FPS = std::to_string(int(ceil(fps)));
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f,
		(float)width / (float)height, 45.0f);
	if (scenenom == 1 || scenenom == -1) {
		glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		DrawSkybox();
		DrawHeightmap();
		DrawWater();
		DrawSun();
		SetCurrentShader(lightShader);
		SetShaderLight(*light);
		DrawRoad();
		for (int i = 0; i < 2; ++i) {
			DrawBuilding1(i);
			DrawBuilding2(i);
			DrawBuilding3(i);
			DrawBuilding4(i);
		}
		glUseProgram(0);
	}
	
	else {
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		DrawSkybox();
		DrawHeightmap();
		DrawSun();
		DrawWater();
		DrawShadowScene();
		DrawCombinedScene();
	}

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_P)) {
		pause != pause;
		pause ? pause = false : pause = true;
	}


	float rtime = 0;
	if (pause == false) {
		if (fps == 0)
			rtime = 0;
		else
			rtime = time / 60;
		float temp = floor(rtime);
		float temp2 = std::modf(rtime, &temp);
		w1 = temp2;
	}
	if ((rtime < 5) && (ptch < 30) && (rtime > 0)) {
		ptch = ptch + 0.23;
		camera->SetPitch(ptch);
	}
	if ((5 < rtime) && (rtime <= 10)) {
		scenenom = 1;
		camera->SetPosition(Vector3(3300,
			500.0f, 1500));
		camera->SetPitch(15);
		camera->SetYaw(-110);
	}
	else if (rtime > 10) {
		scenenom = 2;
		camera->SetPosition(Vector3(7500,
			470, 2000));
		camera->SetPitch(15);
		camera->SetYaw(90);
	}

	if (rtime ==10)
		hkpos.x = 3000;

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_RIGHT))
		scenenom = (scenenom + 1) % 3;
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_LEFT))
		scenenom = (scenenom - 1) % 3;

	if (((Window::GetKeyboard()->KeyTriggered(KEYBOARD_RIGHT)) || (Window::GetKeyboard()->KeyTriggered(KEYBOARD_LEFT))) && scenenom == 0) {
		camera->SetPosition(Vector3(RAW_WIDTH * HEIGHTMAP_X / 2.0f + 700,
			500.0f, RAW_WIDTH * HEIGHTMAP_X));
		camera->SetPitch(-60);
		camera->SetYaw(0);

	}
	else if (((Window::GetKeyboard()->KeyTriggered(KEYBOARD_RIGHT)) || (Window::GetKeyboard()->KeyTriggered(KEYBOARD_LEFT))) && (scenenom == 1 || scenenom == -1)) {
		camera->SetPosition(Vector3(3300,
			500.0f, 1500));
		camera->SetPitch(15);
		camera->SetYaw(-100);
		ufopos.x = 10000;
	}
	else if (((Window::GetKeyboard()->KeyTriggered(KEYBOARD_RIGHT)) || (Window::GetKeyboard()->KeyTriggered(KEYBOARD_LEFT))) && (scenenom == 2 || scenenom == -2)) {
		camera->SetPosition(Vector3(7500,
			470, 2000));
		camera->SetPitch(15);
		camera->SetYaw(90);
		hkpos.x = 3000;
	}


	if ((scenenom == 1 || scenenom == -1)) {
		DrawUfo();
		if (ufopos.x > 3000)
			ufopos.x = ufopos.x - 10;
	}
	if ((scenenom == 2 || scenenom == -2)) {
		ufopos.x = 3000;
		DrawUfo();
		DrawLaser();
		DrawEmitter();
		
		if (hellNode->get_frame() != 37)
			DrawHellKnight(hkpos);
		if (hellNode->get_frame() == 37)
			DrawHellKnight(trsvec);
	}

	if ((5 >= rtime) && (rtime > 4))
		Scenefade(w1);

	if ((10 >= rtime) && (rtime > 9))
		Scenefade(w1);

	textureMatrix.ToIdentity();
	UpdateShaderMatrices();
	DrawText(FPS, Vector3(0, 0, 0), 16.0f);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if ((scenenom == 1 || scenenom == -1)) {
		DrawPostProcess();
		PresentScene();
	}

	SwapBuffers();
}

void Renderer_Scene::DrawPostProcess() {
	glBindFramebuffer(GL_FRAMEBUFFER, processFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, bufferColourTex[1], 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	SetCurrentShader(processShader);
	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
	viewMatrix.ToIdentity();
	modelMatrix.ToIdentity();
	UpdateShaderMatrices();

	glDisable(GL_DEPTH_TEST);

	glUniform2f(glGetUniformLocation(currentShader->GetProgram(),
		"pixelSize"), 1.0f / width, 1.0f / height);
	for (int i = 0; i < POST_PASSES; ++i) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D, bufferColourTex[1], 0);
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
			"isVertical"), 0);

		pproc->SetTexture(bufferColourTex[0]);
		pproc->Draw();

		glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
			"isVertical"), 1);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D, bufferColourTex[0], 0);

		pproc->SetTexture(bufferColourTex[1]);
		pproc->Draw();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);

	glEnable(GL_DEPTH_TEST);
}

void Renderer_Scene::PresentScene() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	SetCurrentShader(lightShader);
	SetShaderLight(*light);
	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
	viewMatrix.ToIdentity();
	UpdateShaderMatrices();
	pproc->SetTexture(bufferColourTex[0]);
	pproc->Draw();
	glUseProgram(0);
}

void Renderer_Scene::DrawSkybox() {		
	glDepthMask(GL_FALSE);
	SetCurrentShader(skyboxShader);

	UpdateShaderMatrices();
	quad->Draw();

	glUseProgram(0);
	glDepthMask(GL_TRUE);
}

void Renderer_Scene::DrawHeightmap() {		
	SetCurrentShader(lightShader);
	SetShaderLight(*light);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)&camera->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"bumpTex"), 1);

	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();

	UpdateShaderMatrices();

	heightMap->Draw();

	glUseProgram(0);
}

void Renderer_Scene::DrawSun() {
	SetCurrentShader(lightShader);			
	SetShaderLight(*light);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)&camera->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"bumpTex"), 1);

	modelMatrix =
		Matrix4::Translation(Vector3(14000.0f, 3000.0f,
			5000.0f)) *
		Matrix4::Scale(Vector3(400, 400, 400)) *
		Matrix4::Rotation(180, Vector3(1.0f, 0.0f, 1.0f));

	textureMatrix.ToIdentity();
	UpdateShaderMatrices();

	sun->Draw();

	glUseProgram(0);
}

void Renderer_Scene::DrawWater() {
	SetCurrentShader(reflectShader);		
	SetShaderLight(*light);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)&camera->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"cubeTex"), 2);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

	float heightX = (RAW_WIDTH * HEIGHTMAP_X / 2.0f);

	float heightY = 256 * HEIGHTMAP_Y / 3.0f;

	float heightZ = (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f);

	modelMatrix =
		Matrix4::Translation(Vector3(heightX, heightY, heightZ)) *
		Matrix4::Scale(Vector3(heightX, 1, heightZ)) *
		Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

	textureMatrix = Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) *
		Matrix4::Rotation(waterRotate, Vector3(0.0f, 0.0f, 1.0f));

	UpdateShaderMatrices();

	quad->Draw();

	glUseProgram(0);
}

void Renderer_Scene::DrawRoad() {
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)&camera->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"cubeTex"), 2);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);


	modelMatrix =
		Matrix4::Translation(Vector3(3500, 350, 2000)) *
		Matrix4::Scale(Vector3(3250, 1, 1000)) *
		Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

	textureMatrix.ToIdentity();

	UpdateShaderMatrices();

	road->Draw();
}

void Renderer_Scene::DrawUfo() {
	SetCurrentShader(lightShader);
	SetShaderLight(*light);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)&camera->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"cubeTex"), 2);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

	modelMatrix =
		Matrix4::Translation(ufopos) *
		Matrix4::Scale(Vector3(2000, 2000, 2000)) *
		Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

	textureMatrix.ToIdentity();
	UpdateShaderMatrices();

	ufo->Draw();

	glUseProgram(0);
}

void Renderer_Scene::DrawLaser() {
	SetCurrentShader(lightShader);
	SetShaderLight(*light);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)&camera->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"cubeTex"), 2);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

	modelMatrix =
		Matrix4::Translation(Vector3(3000, 1000, 2000)) *
		Matrix4::Scale(Vector3(150, 2000, 150)) *
		Matrix4::Rotation(0, Vector3(1.0f, 1.0f, 1.0f));

	textureMatrix = Matrix4::Rotation(laserRotate, Vector3(0.0f, 1.0f, 1.0f));
	UpdateShaderMatrices();

	laser->Draw();

	glUseProgram(0);
}

void Renderer_Scene::DrawBuilding1(int i) {
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)&camera->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"bumpTex"), 1);

	modelMatrix =
		Matrix4::Translation(Vector3(1000 + 2800 * i, 700, 1000)) *
		Matrix4::Scale(Vector3(350, 1000, 350)) *
		Matrix4::Rotation(0, Vector3(1.0f, 1.0f, 1.0f));
	textureMatrix.ToIdentity();

	UpdateShaderMatrices();

	building1->Draw();

	modelMatrix =
		Matrix4::Translation(Vector3(2400 + 2800 * i, 700, 3000)) *
		Matrix4::Scale(Vector3(350, 900, 350)) *
		Matrix4::Rotation(0, Vector3(1.0f, 1.0f, 1.0f));
	textureMatrix.ToIdentity();

	UpdateShaderMatrices();
	building1->Draw();
}

void Renderer_Scene::DrawBuilding2(int i) {
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)&camera->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"bumpTex"), 1);

	modelMatrix =
		Matrix4::Translation(Vector3(1700 + 2800 * i, 700, 1000)) *
		Matrix4::Scale(Vector3(350, 1500, 350)) *
		Matrix4::Rotation(0, Vector3(1.0f, 1.0f, 1.0f));
	textureMatrix.ToIdentity();

	UpdateShaderMatrices();

	building2->Draw();

	modelMatrix =
		Matrix4::Translation(Vector3(3100 + 2800 * i, 700, 3000)) *
		Matrix4::Scale(Vector3(350, 700, 350)) *
		Matrix4::Rotation(0, Vector3(1.0f, 1.0f, 1.0f));
	textureMatrix.ToIdentity();

	UpdateShaderMatrices();
	building2->Draw();
}

void Renderer_Scene::DrawBuilding3(int i) {
glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)&camera->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"bumpTex"), 1);

	modelMatrix =
		Matrix4::Translation(Vector3(2400 + 2800 * i, 700, 1000)) *
		Matrix4::Scale(Vector3(350, 1250, 350)) *
		Matrix4::Rotation(0, Vector3(1.0f, 1.0f, 1.0f));
	textureMatrix.ToIdentity();

	UpdateShaderMatrices();

	building3->Draw();

	modelMatrix =
		Matrix4::Translation(Vector3(1000 + 2800 * i, 700, 3000)) *
		Matrix4::Scale(Vector3(350, 800, 350)) *
		Matrix4::Rotation(0, Vector3(1.0f, 1.0f, 1.0f));
	textureMatrix.ToIdentity();

	UpdateShaderMatrices();
	building3->Draw();
}

void Renderer_Scene::DrawBuilding4(int i) {
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)&camera->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"bumpTex"), 1);

	modelMatrix =
		Matrix4::Translation(Vector3(3100 + 2800 * i, 700, 1000)) *
		Matrix4::Scale(Vector3(350, 1100, 350)) *
		Matrix4::Rotation(0, Vector3(1.0f, 1.0f, 1.0f));
	textureMatrix.ToIdentity();

	UpdateShaderMatrices();

	building4->Draw();

	modelMatrix =
		Matrix4::Translation(Vector3(1700 + 2800 * i, 700, 3000)) *
		Matrix4::Scale(Vector3(350, 1000, 350)) *
		Matrix4::Rotation(0, Vector3(1.0f, 1.0f, 1.0f));
	textureMatrix.ToIdentity();

	UpdateShaderMatrices();
	building4->Draw();
}

void Renderer_Scene::DrawEmitter() {
	SetCurrentShader(particleShader);
	glUseProgram(currentShader->GetProgram());
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)&camera->GetPosition());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	SetShaderParticleSize(emitter->GetParticleSize());

	emitter->SetParticleSize(16.0f);
	emitter->SetParticleVariance(1.0f);
	emitter->SetLaunchParticles(120.0f);
	emitter->SetParticleLifetime(500.0f);
	emitter->SetParticleSpeed(2.0f);

	modelMatrix = Matrix4::Translation(Vector3(3000, 350.0f, 2000));

	UpdateShaderMatrices();

	emitter->Draw();
	glDisable(GL_BLEND);
	glUseProgram(0);
}

void Renderer_Scene::SetShaderParticleSize(float f) {
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "particleSize"), f);
}

void Renderer_Scene::DrawHellKnight(Vector3 pos) {
	//SetCurrentShader(hellShader);
	glUseProgram(currentShader->GetProgram());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
	modelMatrix =
		Matrix4::Translation(hkpos) *
		Matrix4::Scale(Vector3(3, 3, 3)) *
		Matrix4::Rotation(180, Vector3(0.0f, 1.0f, 0.0f));
	UpdateShaderMatrices();

	hellNode->Draw(*this);

	glUseProgram(0);
}

void Renderer_Scene::DrawText(const std::string &text, const Vector3 &position, const float size, const bool perspective) {		//get average framerate
	SetCurrentShader(textShader);
	glUseProgram(currentShader->GetProgram());	

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	
	TextMesh* mesh = new TextMesh(text, *basicFont);

	modelMatrix = Matrix4::Translation(Vector3(position.x, height - position.y, position.z)) * Matrix4::Scale(Vector3(size, size, 1));
	viewMatrix.ToIdentity();
	projMatrix = Matrix4::Orthographic(-1.0f, 1.0f, (float)width, 0.0f, (float)height, 0.0f);

	
	UpdateShaderMatrices();
	mesh->Draw();

	delete mesh; 
	glUseProgram(0);
}

void Renderer_Scene::DrawShadowScene() {
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);

	glClear(GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	SetCurrentShader(shadowShader);

	viewMatrix = Matrix4::BuildViewMatrix(
		light->GetPosition(), Vector3(0, 0, 0));
	shadowViewProj = biasMatrix *(projMatrix * viewMatrix);

	UpdateShaderMatrices();

	DrawRoad();

	for (int i = 0; i < 2; ++i) {
		DrawBuilding1(i);
		DrawBuilding2(i);
		DrawBuilding3(i);
		DrawBuilding4(i);
	}
	glUseProgram(0);

	SetCurrentShader(hellShader);
	DrawHellKnight(hkpos);

	glUseProgram(0);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer_Scene::DrawCombinedScene() {
	SetCurrentShader(sceneShader);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"bumpTex"), 1);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"shadowTex"), 2);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)&camera->GetPosition());


	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "shadowProjMatrix"), 1, false, (float*)&shadowViewProj);
	SetShaderLight(*light);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowTex);

	viewMatrix = camera->BuildViewMatrix();
	UpdateShaderMatrices();

	DrawRoad();
	for (int i = 0; i < 2; ++i) {
		DrawBuilding1(i);
		DrawBuilding2(i);
		DrawBuilding3(i);
		DrawBuilding4(i);
	}

	glUseProgram(0);

	SetCurrentShader(hellShader);
	DrawHellKnight(hkpos);
	glUseProgram(0);
}

void Renderer_Scene::Scenefade(float w) {
	Mesh* fade = Mesh::GenerateTriangle(w);

	SetCurrentShader(fadeShader);
	glUseProgram(currentShader->GetProgram());
	glEnable(GL_BLEND);
	fade->Draw();

	delete fade;
	glUseProgram(0);
}