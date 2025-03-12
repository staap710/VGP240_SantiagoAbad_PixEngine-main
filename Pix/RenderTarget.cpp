#include "RenderTarget.h"

void RenderTarget::Initialize(int width, int height, const X::Color& clearColor)
{
	if (mWidth != width || mHeight != height)
	{
		mWidth = width;
		mHeight = height;
		mRenderPixels.reset();
		mRenderPixels = std::make_unique<X::Color[]>(mWidth * mHeight);
	}


}
void RenderTarget::ClearImage(const X::Color& clearColor)
{	
	for (int h = 0; h < mHeight; ++h)
	{
		for (int w = 0; w < mWidth; ++w)
		{
			const int Index = w + (h * mWidth);
			mRenderPixels[index] = clearColor;
		}
	}

}
void RenderTarget::DrawPixel(int x, int y, const X::Color& color)
{
	if (x >= 0 && x < mWidth && y >= 0 && y < mHeight)
	{
		const int index = x + (y * mWidth);
		mRenderPixels[index] = color;
	}
}
const RenderTarget::X::Color& RenderTarget::GetColor(int x, int y) const
{
	x = std::clamp(x, 0, mWidth - 1);
	y = std::clamp(y, 0, mHeight - 1);
	const int index = x + (y * mWidth);
	return mRenderPixels[index];
}
int RenderTarget::GetWidth() const
{
	return mWidth;
}
int RenderTarget::GetHeight() const
{
	return mHeight;
}


