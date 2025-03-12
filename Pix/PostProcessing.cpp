#include "PostProcessing.h"

PostProcessing* PostProcessing::Get()
{
	static PostProcessing instance;
	return &instance;
}
void PostProcessing::Initialize(int width, int height, const X::Color& clearColor)
{
	mRenderTarget.Initialize(width, height, clearColor);
	mClearColor = clearColor;
}
void PostProcessing::onNewFrame()
{
	mDrawBegin = false;
	mRenderTarget.ClearImage(mClearColor);
}
void PostProcessing::SetEffect(EffectType effectType)
{
	mEffectType = effectType;
}
void PostProcessing::BeginDraw()
{
	mDrawBegin = mEffectType != EffectType::None;
	if (mDrawBegin) {
		mRenderTarget.ClearImage(mClearColor);
	}
}
bool PostProcessing::Draw(int x, int y, const X::Color& color)
{
	if (mDrawBegin)
	{
		mRenderTarget.DrawPixel(x, y, color);
		return true;
	}
	return false;
}
void PostProcessing::EndDraw()
{
	for (int y = 0; y < mRenderTarget.GetHeight(); ++y)
	{
		for (int x = 0; x < mRenderTarget.GetWidth(); ++x)
		{
			DrawPixel(x, y);
		}
	}
	mDrawBegin = false;
}
void PostProcessing::DrawPixel(int x, int y)
{
	X::Color color = mRenderTarget.GetColor(x, y);
	X::DrawPixel(x, y, color);
}
void PostProcessing::SetColor(X::Color color)
{
	mColor = color;
}
void PostProcessing::SetFillMode(FillMode fillMode)
{
	mFillMode = fillMode;
}