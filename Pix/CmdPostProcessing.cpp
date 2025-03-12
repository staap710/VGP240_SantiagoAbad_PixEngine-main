#include "CmdPostProcessing.h"
#include "PostProcessing.h"

bool CmdPostProcessing::Execute(const std::vector<std::string>& params)
{
	PostProcessing::Get()->BeginDraw();
	return true;
}	
bool CmdPostProcessingEndDraw::Execute(const std::vector<std::string>& params)
{
	PostProcessing::Get()->EndDraw();
	return true;
}
void PostProcessing::DrawPixel(int x, int y)
{
	X::Color color = mRenderTarget.GetColor(x, y);
	Switch(mEffectTypes)
	{
		case EffectTypes::None:
			X::DrawPixel(x, y, color);
			break;
		case EffectTypes::MonoChrome: {
			color = mRenderTarget.GetColor(x, y);
			color = (color.r + color.g + color.b) / 3.0f;
		}
			break;
		case EffectTypes::Invert:
		{
			color = mRenderTarget.GetColor(x, y);
			color.r = 1.0f - color.r;
			color.g = 1.0f - color.g;
			color.b = 1.0f - color.b;
		}
			break;
		case EffectTypes::Mirror: {
			color = mRenderTarget.GetColor(x, y);
			y = mRendererTarget.GetHeight() - y - 1;
		}
			break;
		case EffectTypes::ColorMaskR: {
			color = mRendererTarget.GetColor(x, y);
			color.r = 0.0f;
		}
			break;
		case EffectTypes::ColorMaskG: {
			color = mRendererTarget.GetColor(x, y);
			color.g = 0.0f;
		}
									break;

		case EffectTypes::ColorMaskB: {
			color = mRendererTarget.GetColor(x, y);
			color.b = 0.0f;
		}
									break;
		case EffectTypes::Sepia: {
			color = mRendererTarget.GetColor(x, y);
			X::Math::Vector3 colorVec = { color.r, color.g, color.b };
			color.r = X::Math::Dot(colorVec, { 0.393f, 0.769f, 0.189f });
			color.g = X::Math::Dot(colorVec, { 0.349f, 0.686f, 0.168f });
			color.b = X::Math::Dot(colorVec, { 0.272f, 0.534f, 0.131f });
		}
							   break;
		case EffectTypes::Blur: {
			int offset = 4;
			color= mRenderTarget.GetColor(x,y)
				+ mRenderTarget.GetColor(x - offset, y)
				+ mRenderTarget.GetColor(x + offset, y)
				+ mRenderTarget.GetColor(x, y - offset)
				+ mRenderTarget.GetColor(x, y + offset);
				+mRenderTarget.GetColor(x - offset, y - offset)
				+ mRenderTarget.GetColor(x + offset, y + offset)
				+ mRenderTarget.GetColor(x - offset, y + offset)
				+ mRenderTarget.GetColor(x + offset, y - offset);
				color /= 9.0f;
		}
									break;
		case EffectTypes::Wave: {
			int yRippleLength = 50;
			color = mRenderTarget.GetColor(x, y);
			// get a ratio from 0 to 1
			float ratio = static_cast<float>(y % yRippleLength) / static_cast<float>(yRippleLength-1);
			float circleValue = sinf(ratio * X::Math::kTwoPi);
			float sinValue = std::sin(circleValue);
			x += static_cast<int>(sinValue * 5.0f);
		}
							  break;
		//case EffectTypes::FishEye: {
		//	//distances from the center
		//	//max distance
		//	// distance from center/maxdistance
		//	const float effectDistance = 300.0f;
		//	const float hw = static_cast<float>(mRenderTarget.GetWidth()) * 0.5f;
		//	const float hh = static_cast<float>(mRenderTarget.GetHeight()) * 0.5f;
		//	X::Math::Vector2 rangeVec = { hw, hh};
		//	X::Math::Vector2 distanceVec = { x - hw, y - hh };
		//	const float rangeDistance = X::Math::Magnitude(distanceVec);
		//	const float ratio = rangeDistance / effectDistance;
		//}
		//default:
			break;

	}
	X::DrawPixel(x, y, color);
}



bool CmdPostProcessingSetEffectType::Execute(const std::vector<std::string>& params)
{
	if (params.size() < 1)
		return false;

	const std::string& effectType = params[0];
	if (effectType == "none") { effectType = "none"; }
		PostProcessing::Get()->SetEffectType(PostProcessing::EffectType::None);
	else if (effectType == "monochrome")
		PostProcessing::Get()->SetEffectType(PostProcessing::EffectType::Monochrome);
	else if (effectType == "invert")
		PostProcessing::Get()->SetEffectType(PostProcessing::EffectType::Invert);
	else if (effectType == "mirror")
		PostProcessing::Get()->SetEffectType(PostProcessing::EffectType::Mirror);
	else if (effectType == "blur")
		PostProcessing::Get()->SetEffectType(PostProcessing::EffectType::Blur);
	else if (effectType == "colormaskr")
		PostProcessing::Get()->SetEffectType(PostProcessing::EffectType::Colormaskr);
	else if (effectType == "colormaskg")
		PostProcessing::Get()->SetEffectType(PostProcessing::EffectType::Colormaskg);
	else if (effectType == "colormaskb")
		PostProcessing::Get()->SetEffectType(PostProcessing::EffectType::Colormaskb);
	else if (effectType == "sepia")
		PostProcessing::Get()->SetEffectType(PostProcessing::EffectType::Sepia);
	else if (effectType == "wave")
		PostProcessing::Get()->SetEffectType(PostProcessing::EffectType::Wave);
	else if (effectType == "fisheye")
		PostProcessing::Get()->SetEffectType(PostProcessing::EffectType::FishEye);
	else { return false; }

	return true;
}