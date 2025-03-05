#include "Texture.h"

void Texture::Load(const std::string& fileName) {

	mFileName = fileName;
	FILE* file = nullptr;
	fopen_s(&file, fileName.c_str(), "rb");
	if (file == nullptr)
	{
		char buffer[128];
		sprintf_s(buffer, "Failed to open file: %s", fileName.c_str());
		MessageBox(nullptr, buffer, "tEXTURE Error", MB_OK);

	}
}
const std::string& GetFilename() const;
X::Color GetPixel(int u, int v) const;
int GetWidth() const;
int GetHeight() const;