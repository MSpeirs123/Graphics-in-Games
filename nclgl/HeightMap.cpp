#include "HeightMap.h"



HeightMap::HeightMap(std::string name) {
	std::ifstream file(name.c_str(), ios::binary);
	if (!file) {
		return;
	}
	numVerticies = RAW_WIDTH * RAW_HEIGHT;
	numIndicies = (RAW_WIDTH - 1)*(RAW_HEIGHT - 1) * 6;
	verticies = new Vector3[numVerticies];
	textureCoords = new Vector2[numVerticies];
	indicies = new GLuint[numIndicies];

	unsigned char * data = new unsigned char[numVerticies];
	file.read((char*)data, numVerticies * sizeof(unsigned char));
	file.close();

	for (int x = 0; x < RAW_WIDTH; ++x) {
		for (int z = 0; z < RAW_HEIGHT; ++z) {
			int offset = (x * RAW_WIDTH) + z;

			verticies[offset] = Vector3(
				x * HEIGHTMAP_X, data[offset] * HEIGHTMAP_Y, z * HEIGHTMAP_Z);

			textureCoords[offset] = Vector2(
				x * HEIGHTMAP_TEX_X, z * HEIGHTMAP_TEX_Z);
		}
	}

	delete data;

	numIndicies = 0;

	for (int x = 0; x < RAW_WIDTH - 1; ++x) {
		for (int z = 0; z < RAW_HEIGHT - 1; ++z) {
			int a = (x * (RAW_WIDTH)) + z;
			int b = ((x + 1) * (RAW_WIDTH)) + z;
			int c = ((x + 1) * (RAW_WIDTH)) + (z + 1);
			int d = (x * (RAW_WIDTH)) + (z + 1);

			indicies[numIndicies++] = c;
			indicies[numIndicies++] = b;
			indicies[numIndicies++] = a;

			indicies[numIndicies++] = a;
			indicies[numIndicies++] = d;
			indicies[numIndicies++] = c;
		}
	}
	GenerateNormals();
	GenerateTangents();
	BufferData();
}
