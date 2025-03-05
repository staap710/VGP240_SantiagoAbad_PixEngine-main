#pragma once

#include <XEngine.h>

class MyClass
{
public:
	void Load(const std::string& fileName);
	const std::string& GetFilename() const;
	X::Color GetPixel(int u, int v) const;
	int GetWidth() const;
	int GetHeight() const;
private:
	X::Color GetPixel(int u, int v) const;
	std::string mFileName;
	std::unique_ptr<X::Color[]> mPixels;
	X::TextureId mTextureId = 0;
	int mWidth = 0;
	int mHeight = 0;

};

