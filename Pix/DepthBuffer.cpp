#include "DepthBuffer.h"

DepthBuffer* DepthBuffer::Get()
{
	static DepthBuffer sInstance;
	return &sInstance;
}

void DepthBuffer::OnNewFrame()
{
	mEnabled = false;
	for (int i = 0; i < mWidth * mHeight; ++i) {
		mDepthBuffer[i] = 1.0f;
	}
}

void DepthBuffer::Initialize(int width, int height)
{
	if (width != mWidth || height != mHeight) {
		mWidth = width;
		mHeight = height;
		mDepthBuffer = std::make_unique<float[]>(width * height);
		OnNewFrame();
	}
}	

void DepthBuffer::SetEnabled(bool enabled)
{
	mEnabled = enabled;
}

bool DepthBuffer::CheckDepthBuffer(int x, int y, float depth)
{
	if (!mEnabled) {
		return true;
	}

	if (x < 0 || x >= mWidth || y < 0 || y >= mHeight) {
		return false;
	}

	int index = x + y * mWidth;
	if (depth < mDepthBuffer[index]) {
		mDepthBuffer[index] = depth;
		return true;
	}

	return false;
}