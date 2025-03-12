#pragma once
#include "RenderTarget.h"
enum class EffectTypes {
	None,
	MonoChrome,
	Invert,
	Mirror,
};

class PostProcessing final {
public:
	static PostProcessing* Get();

	void Initialize(int width, int height, const X::Color& clearColor=X::Colors::Black);
	void onNewFrame();
	void SetEffect(mEffectType effectType);
	void BeginDraw();
	bool Draw(int x, int y, const X::Color& color);
	void EndDraw();
private:
	void DrawPixel(int x, int y);
	bool mDrawBegin = false;
	X::Color mClearColor = X::Colors::Black;
	RenderTarget mRenderTarget;
};