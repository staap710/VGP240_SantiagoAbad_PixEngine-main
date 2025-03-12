#pragma once

#include <xEngine.h>

class RenderTarget
{
public:
	void Initialize(int width, int height, const X::Color& clearColor= X::Colors::Black);
	void ClearImage(const X::Color& clearColor);
	void DrawPixel(int x, int y, const X::Color& color);
	const X::Color& GetColor(int x, int y) const;
	
	int GetWidth() const;
	int GetHeight() const;
private:
	std::unique_ptr<X::Color[]> mRenderPixels;
	int mWidth = 0;
	int mHeight = 0;
};