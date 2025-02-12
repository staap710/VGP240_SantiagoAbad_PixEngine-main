#include "Graphics.h"

#include "Viewport.h"
#include "Clipper.h"
#include "MatrixStack.h"
#include "Camera.h"
#include "PrimativesManager.h"
#include "DepthBuffer.h"

void Graphics::NewFrame()
{
	Viewport::Get()->OnNewFrame();
	Clipper::Get()->OnNewFrame();
	MatrixStack::Get()->OnNewFrame();
	Camera::Get()->OnNewFrame();
	PrimativesManager::Get()->OnNewFrame();
	DepthBuffer::Get()->OnNewFrame();
}