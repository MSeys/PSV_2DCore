#include "pch.h"
#include "Texture.h"

Texture::Texture(const std::string& fileName)
	: m_pVitaTexture{ vita2d_load_PNG_file(fileName.c_str()) }
{
}

Texture::~Texture()
{
	vita2d_free_texture(m_pVitaTexture);
	m_pVitaTexture = nullptr;
}

void Texture::Draw(const Point2f& pos, const Rectf& srcRect, const Scale2f& scales, float angle, const Point2f& hotspot) const
{
	Rectf trueSrcRect{ srcRect };
	// full texture
	if (srcRect.width == 0 || srcRect.height == 0)
	{
		trueSrcRect.width = GetWidth();
		trueSrcRect.height = GetHeight();
	}

	if(PSV_Allowed)
	{
		if(angle != 0)
		{
			if(hotspot.x != -1 && hotspot.y != -1)
			{
				vita2d_draw_texture_part_scale_rotate_hotspot(m_pVitaTexture,
					GetTransformedPoint(pos).x, GetTransformedPoint(pos).y,
					trueSrcRect.left, trueSrcRect.bottom, trueSrcRect.width, trueSrcRect.height,
					PSV_Scales[PSV_CT].x * scales.x, PSV_Scales[PSV_CT].y * scales.y,
					angle, hotspot.x, hotspot.y);
			}

			else
			{
				vita2d_draw_texture_part_scale_rotate(m_pVitaTexture, 
					GetTransformedPoint(pos).x, GetTransformedPoint(pos).y,
					trueSrcRect.left, trueSrcRect.bottom, trueSrcRect.width, trueSrcRect.height,
					PSV_Scales[PSV_CT].x * scales.x, PSV_Scales[PSV_CT].y * scales.y, 
					angle);
			}
		}

		else
		{
			vita2d_draw_texture_part_scale(m_pVitaTexture, 
				GetTransformedPoint(pos).x, GetTransformedPoint(pos).y,
				trueSrcRect.left, trueSrcRect.bottom, trueSrcRect.width, trueSrcRect.height,
				PSV_Scales[PSV_CT].x * scales.x, PSV_Scales[PSV_CT].y * scales.y);
		}
	}

	else
	{
		if (angle != 0)
		{
			if (hotspot.x != 0.01 && hotspot.y != 0.01)
			{
				vita2d_draw_texture_part_scale_rotate_hotspot(m_pVitaTexture,
					pos.x, pos.y,
					trueSrcRect.left, trueSrcRect.bottom, trueSrcRect.width, trueSrcRect.height,
					scales.x, scales.y,
					angle, hotspot.x, hotspot.y);
			}

			else
			{
				vita2d_draw_texture_part_scale_rotate(m_pVitaTexture,
					pos.x, pos.y,
					trueSrcRect.left, trueSrcRect.bottom, trueSrcRect.width, trueSrcRect.height,
					scales.x, scales.y,
					angle);
			}
		}

		else
		{
			vita2d_draw_texture_part_scale(m_pVitaTexture, pos.x, pos.y,
				trueSrcRect.left, trueSrcRect.bottom, trueSrcRect.width, trueSrcRect.height,
				scales.x, scales.y);
		}
	}
}

void Texture::Draw(const Point2f& pos, float angle, const Point2f& hotspot, const Rectf& srcRect,
	const Scale2f& scales) const
{
	Draw(pos, srcRect, scales, angle, hotspot);
}

float Texture::GetWidth() const
{
	return vita2d_texture_get_width(m_pVitaTexture);
}

float Texture::GetHeight() const
{
	return vita2d_texture_get_height(m_pVitaTexture);
}
