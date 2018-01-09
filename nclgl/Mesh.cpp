#include "Mesh.h"



Mesh::Mesh(void)
{
	for (int i = 0; i < MAX_BUFFER; ++i) {
		bufferObject[i] = 0;
	}
	glGenVertexArrays(1, &arrayObject);
	numVerticies = 0;
	verticies = NULL;
	numIndicies = 0;
	indicies = NULL;
	colours = NULL;
	type = GL_TRIANGLES;
	texture = 0;
	textureCoords = NULL;
	normals = NULL;
	tangents = NULL;
	bumpTexture = 0;
}


Mesh::~Mesh(void)
{
	glDeleteVertexArrays(1, &arrayObject);
	glDeleteBuffers(MAX_BUFFER, bufferObject);
	glDeleteTextures(1, &texture);
	glDeleteTextures(1, &bumpTexture);
	delete[] verticies;
	delete[] colours;
	delete[] textureCoords;
	delete[] indicies;
	delete[] normals;
	delete[] tangents;
}

Mesh * Mesh::GenerateTriangle(float w) {
	Mesh * m = new Mesh();
	m->numVerticies = 3;
	m->verticies = new Vector3[m->numVerticies];
	m->verticies[0] = Vector3(0.0f, 10.0f, 0.0f);
	m->verticies[1] = Vector3(5.5f, -1.0f, 0.0f);
	m->verticies[2] = Vector3(-5.5f, -1.0f, 0.0f);

	m->textureCoords = new Vector2[m->numVerticies];
	m->textureCoords[0] = Vector2(0.5f, 0.0f);
	m->textureCoords[1] = Vector2(1.0f, 1.0f);
	m->textureCoords[2] = Vector2(0.0f, 1.0f);


	m->colours = new Vector4[m->numVerticies];
	m->colours[0] = Vector4(0.0f, 0.0f, 0.0f, w);
	m->colours[1] = Vector4(0.0f, 0.0f, 0.0f, w);
	m->colours[2] = Vector4(0.0f, 0.0f, 0.0f, w);

	m->BufferData();
	return m;
}

Mesh* Mesh::GenerateQuad() {
	Mesh* m = new Mesh();
	m->numVerticies = 4;
	m->type = GL_TRIANGLE_STRIP;

	m->verticies = new Vector3[m->numVerticies];
	m->textureCoords = new Vector2[m->numVerticies];
	m->colours = new Vector4[m->numVerticies];
	m->normals = new Vector3[m->numVerticies];
	m->tangents = new Vector3[m->numVerticies];

	m->verticies[0] = Vector3(-1.0f, -1.0f, 0.0f);
	m->verticies[1] = Vector3(-1.0f, 1.0f, 0.0f);
	m->verticies[2] = Vector3(1.0f, -1.0f, 0.0f);	
	m->verticies[3] = Vector3(1.0f, 1.0f, 0.0f);

	m->textureCoords[0] = Vector2(0.0f, 1.0f);
	m->textureCoords[1] = Vector2(0.0f, 0.0f);
	m->textureCoords[2] = Vector2(1.0f, 1.0f);
	m->textureCoords[3] = Vector2(1.0f, 0.0f);

	
	for (int i = 0; i < m->numVerticies; ++i) {
		m->colours[i] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		m->normals[i] = Vector3(0.0f, 0.0f, -1.0f);
		m->tangents[i] = Vector3(1.0f, 1.0f, 0.0f);
	}

	m->BufferData();
	return m;
}

Mesh* Mesh::GenerateCircle() {
	Mesh* m = new Mesh();
	m->numVerticies = 361;
	m->type = GL_TRIANGLE_FAN;

	m->verticies = new Vector3[m->numVerticies];
	m->textureCoords = new Vector2[m->numVerticies];
	m->colours = new Vector4[m->numVerticies];
	m->normals = new Vector3[m->numVerticies];
	m->tangents = new Vector3[m->numVerticies];

	m->verticies[0] = Vector3(0.0f, 0.0f, 0.0f);
	m->textureCoords[0] = Vector2(0.0f, 0.0f);
	m->colours[0] = Vector4(1.0f, 1.0f, 1.0f, 0.5f);
	m->normals[0] = Vector3(0.0f, 0.0f, -1.0f);
	m->tangents[0] = Vector3(1.0f, 1.0f, 0.0f);
	for (int i = 0; i < 360; ++i) {
		m->verticies[i + 1] = Vector3(cos(i), sin(i), 0.0f);
		m->textureCoords[i + 1] = Vector2(cos(i), sin(i));
		m->colours[i] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		m->normals[i] = Vector3(0.0f, 0.0f, -1.0f);
		m->tangents[i] = Vector3(1.0f, 1.0f, 0.0f);
	}

	m->BufferData();
	return m;
}



Mesh* Mesh::GenerateCube() {
	Mesh* m = new Mesh();
	m->numVerticies = 20;
	m->type = GL_TRIANGLE_STRIP;

	m->verticies = new Vector3[m->numVerticies];
	m->textureCoords = new Vector2[m->numVerticies];
	m->colours = new Vector4[m->numVerticies];
	m->normals = new Vector3[m->numVerticies];
	m->tangents = new Vector3[m->numVerticies];

	m->verticies[0] = Vector3(-1.0f, -1.0f, -1.0f);
	m->verticies[1] = Vector3(-1.0f, 1.0f, -1.0f);
	m->verticies[2] = Vector3(1.0f, -1.0f, -1.0f);
	m->verticies[3] = Vector3(1.0f, 1.0f, -1.0f);

	m->verticies[4] = Vector3(1.0f, -1.0f, -1.0f);
	m->verticies[5] = Vector3(1.0f, 1.0f, -1.0f);
	m->verticies[6] = Vector3(1.0f, -1.0f, 1.0f);
	m->verticies[7] = Vector3(1.0f, 1.0f, 1.0f);

	m->verticies[8] = Vector3(1.0f, -1.0f, 1.0f);
	m->verticies[9] = Vector3(1.0f, 1.0f, 1.0f);
	m->verticies[10] = Vector3(-1.0f, -1.0f, 1.0f);
	m->verticies[11] = Vector3(-1.0f, 1.0f, 1.0f);

	m->verticies[12] = Vector3(-1.0f, -1.0f, 1.0f);
	m->verticies[13] = Vector3(-1.0f, 1.0f, 1.0f);
	m->verticies[14] = Vector3(-1.0f, -1.0f, -1.0f);
	m->verticies[15] = Vector3(-1.0f, 1.0f, -1.0f);

	m->verticies[16] = Vector3(-1.0f, 1.0f, -1.0f);
	m->verticies[17] = Vector3(1.0f, 1.0f, -1.0f);
	m->verticies[18] = Vector3(-1.0f, 1.0f, 1.0f);
	m->verticies[19] = Vector3(1.0f, 1.0f, 1.0f);

	m->textureCoords[0] = Vector2(0.0f, 1.0f);
	m->textureCoords[1] = Vector2(0.0f, 0.0f);
	m->textureCoords[2] = Vector2(1.0f, 1.0f);
	m->textureCoords[3] = Vector2(1.0f, 0.0f);
	m->textureCoords[4] = Vector2(0.0f, 1.0f);
	m->textureCoords[5] = Vector2(0.0f, 0.0f);
	m->textureCoords[6] = Vector2(1.0f, 1.0f);
	m->textureCoords[7] = Vector2(1.0f, 0.0f);
	m->textureCoords[8] = Vector2(0.0f, 1.0f);
	m->textureCoords[9] = Vector2(0.0f, 0.0f);
	m->textureCoords[10] = Vector2(1.0f, 1.0f);
	m->textureCoords[11] = Vector2(1.0f, 0.0f);
	m->textureCoords[12] = Vector2(0.0f, 1.0f);
	m->textureCoords[13] = Vector2(0.0f, 0.0f);
	m->textureCoords[14] = Vector2(1.0f, 1.0f);
	m->textureCoords[15] = Vector2(1.0f, 0.0f);
	m->textureCoords[16] = Vector2(0.0f, 1.0f);
	m->textureCoords[17] = Vector2(0.0f, 0.0f);
	m->textureCoords[18] = Vector2(1.0f, 1.0f);
	m->textureCoords[19] = Vector2(1.0f, 0.0f);

	m->normals[0] = Vector3(0.0f, 0.0f, -1.0f);
	m->normals[1] = Vector3(0.0f, 0.0f, -1.0f);
	m->normals[2] = Vector3(0.0f, 0.0f, -1.0f);
	m->normals[3] = Vector3(0.0f, 0.0f, -1.0f);

	m->normals[4] = Vector3(1.0f, 0.0f, 0.0f);
	m->normals[5] = Vector3(1.0f, 0.0f, 0.0f);
	m->normals[6] = Vector3(1.0f, 0.0f, 0.0f);
	m->normals[7] = Vector3(1.0f, 0.0f, 0.0f);

	m->normals[8] = Vector3(0.0f, 0.0f, 1.0f);
	m->normals[9] = Vector3(0.0f, 0.0f, 1.0f);
	m->normals[10] = Vector3(0.0f, 0.0f, 1.0f);
	m->normals[11] = Vector3(0.0f, 0.0f, 1.0f);

	m->normals[12] = Vector3(-1.0f, 0.0f, 0.0f);
	m->normals[13] = Vector3(-1.0f, 0.0f, 0.0f);
	m->normals[14] = Vector3(-1.0f, 0.0f, 0.0f);
	m->normals[15] = Vector3(-1.0f, 0.0f, 0.0f);

	m->normals[16] = Vector3(0.0f, 1.0f, 0.0f);
	m->normals[17] = Vector3(0.0f, 1.0f, 0.0f);
	m->normals[18] = Vector3(0.0f, 1.0f, 0.0f);
	m->normals[19] = Vector3(0.0f, 1.0f, 0.0f);


	m->tangents[0] = Vector3(1.0f, 1.0f, 0.0f);
	m->tangents[1] = Vector3(1.0f, 1.0f, 0.0f);
	m->tangents[2] = Vector3(1.0f, 1.0f, 0.0f);
	m->tangents[3] = Vector3(1.0f, 1.0f, 0.0f);

	m->tangents[4] = Vector3(0.0f, 1.0f, 1.0f);
	m->tangents[5] = Vector3(0.0f, 1.0f, 1.0f);
	m->tangents[6] = Vector3(0.0f, 1.0f, 1.0f);
	m->tangents[7] = Vector3(0.0f, 1.0f, 1.0f);

	m->tangents[8] = Vector3(-1.0f, -1.0f, 0.0f);
	m->tangents[9] = Vector3(-1.0f, -1.0f, 0.0f);
	m->tangents[10] = Vector3(-1.0f, -1.0f, 0.0f);
	m->tangents[11] = Vector3(-1.0f, -1.0f, 0.0f);

	m->tangents[12] = Vector3(0.0f, -1.0f, -1.0f);
	m->tangents[13] = Vector3(0.0f, -1.0f, -1.0f);
	m->tangents[14] = Vector3(0.0f, -1.0f, -1.0f);
	m->tangents[15] = Vector3(0.0f, -1.0f, -1.0f);

	m->tangents[16] = Vector3(1.0f, 0.0f, 1.0f);
	m->tangents[17] = Vector3(1.0f, 0.0f, 1.0f);
	m->tangents[18] = Vector3(1.0f, 0.0f, 1.0f);
	m->tangents[19] = Vector3(1.0f, 0.0f, 1.0f);

	for (int i = 0; i < m->numVerticies; ++i) {
		m->colours[i] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	m->BufferData();
	return m;
}

void Mesh::GenerateNormals() {
	if (!normals) {
		normals = new Vector3[numVerticies];
	}
	for (GLuint i = 0; i < numVerticies; ++i) {
		normals[i] = Vector3();
	}
	if (indicies) { // Generate per - vertex normals
		for (GLuint i = 0; i < numIndicies; i += 3) {
			unsigned int a = indicies[i];
			unsigned int b = indicies[i + 1];
			unsigned int c = indicies[i + 2];

			Vector3 normal = Vector3::Cross(
				(verticies[b] - verticies[a]), (verticies[c] - verticies[a]));

			normals[a] += normal;
			normals[b] += normal;
			normals[c] += normal;
		}
	}
	else { // It ’s just a list of triangles , so generate face normals
		for (GLuint i = 0; i < numVerticies; i += 3) {
			Vector3 & a = verticies[i];
			Vector3 & b = verticies[i + 1];
			Vector3 & c = verticies[i + 2];

			Vector3 normal = Vector3::Cross(b - a, c - a);

			normals[i] = normal;
			normals[i + 1] = normal;
			normals[i + 2] = normal;
		}
	}

	for (GLuint i = 0; i < numVerticies; ++i) {
		normals[i].Normalise();
	}
}

void Mesh::GenerateTangents() {
	if (!tangents) {
		tangents = new Vector3[numVerticies];
	}
	for (GLuint i = 0; i < numVerticies; ++i) {
		tangents[i] = Vector3();
	}

	if (indicies) {
		for (GLuint i = 0; i < numIndicies; i += 3) {
			int a = indicies[i];
			int b = indicies[i + 1];
			int c = indicies[i + 2];

			Vector3 tangent = GenerateTangent(verticies[a], verticies[b],
				verticies[c], textureCoords[a],
				textureCoords[b], textureCoords[c]);

			tangents[a] += tangent;
			tangents[b] += tangent;
			tangents[c] += tangent;
		}
	}
	else {
		for (GLuint i = 0; i < numVerticies; i += 3) {
			Vector3 tangent = GenerateTangent(verticies[i], verticies[i + 1],
				verticies[i + 2], textureCoords[i],
				textureCoords[i + 1], textureCoords[i + 2]);

			tangents[i] += tangent;
			tangents[i + 1] += tangent;
			tangents[i + 2] += tangent;
		}
	}
	for (GLuint i = 0; i < numVerticies; ++i) {
		tangents[i].Normalise();
	}
}

void Mesh::BufferData() {
	glBindVertexArray(arrayObject);
	glGenBuffers(1, &bufferObject[VERTEX_BUFFER]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[VERTEX_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, numVerticies * sizeof(Vector3),
	verticies, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(VERTEX_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VERTEX_BUFFER);
	if (indicies) {
		glGenBuffers(1, &bufferObject[INDEX_BUFFER]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
		bufferObject[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndicies * sizeof(GLuint),
		indicies, GL_STATIC_DRAW);
	}
	if (textureCoords) {
		glGenBuffers(1, &bufferObject[TEXTURE_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[TEXTURE_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVerticies * sizeof(Vector2),
			textureCoords, GL_STATIC_DRAW);
		glVertexAttribPointer(TEXTURE_BUFFER, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(TEXTURE_BUFFER);
	}
	if (colours) { // Just in case the data has no colour attribute ...
		glGenBuffers(1, &bufferObject[COLOUR_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[COLOUR_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVerticies * sizeof(Vector4),
		colours, GL_STATIC_DRAW);
		glVertexAttribPointer(COLOUR_BUFFER, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(COLOUR_BUFFER);
	}
	if (normals) {
		glGenBuffers(1, &bufferObject[NORMAL_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[NORMAL_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVerticies * sizeof(Vector3),
			normals, GL_STATIC_DRAW);
		glVertexAttribPointer(NORMAL_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(NORMAL_BUFFER);
	}
	if (tangents) {
		glGenBuffers(1, &bufferObject[TANGENT_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[TANGENT_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVerticies * sizeof(Vector3),
			tangents, GL_STATIC_DRAW);
		glVertexAttribPointer(TANGENT_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(TANGENT_BUFFER);
	}

	glBindVertexArray(0);
}

	Vector3 Mesh::GenerateTangent(const Vector3 &a, const Vector3 &b,
		const Vector3 &c, const Vector2 & ta,
		const Vector2 & tb, const Vector2 & tc) {
		Vector2 coord1 = tb - ta;
		Vector2 coord2 = tc - ta;

		Vector3 vertex1 = b - a;
		Vector3 vertex2 = c - a;

		Vector3 axis = Vector3(vertex1 * coord2.y - vertex2 * coord1.y);

		float factor = 1.0f / (coord1.x * coord2.y - coord2.x * coord1.y);

		return axis * factor;
	}

void Mesh::Draw() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(arrayObject);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bumpTexture);
	if (bufferObject[INDEX_BUFFER]) {
		glDrawElements(type, numIndicies, GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(type, 0, numVerticies);
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}