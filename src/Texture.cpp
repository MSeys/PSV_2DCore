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

void Texture::Draw(const Point2f& pos, const Rectf& srcRect, const Scale2f& scale) const
{
	Rectf trueSrcRect{ srcRect };
	// full texture
	if(srcRect.width == 0 || srcRect.height == 0)
	{
		trueSrcRect.width = GetWidth();
		trueSrcRect.height = GetHeight();
	}

	vita2d_draw_texture_part_scale(m_pVitaTexture, pos.x, pos.y, trueSrcRect.left, trueSrcRect.bottom, trueSrcRect.width, trueSrcRect.height, scale.x, scale.y);
}

void Texture::Draw(const Point2& pos, const Rectf& srcRect, const Scale2f& scale) const
{
	Rectf trueSrcRect{ srcRect };
	// full texture
	if (srcRect.width == 0 || srcRect.height == 0)
	{
		trueSrcRect.width = GetWidth();
		trueSrcRect.height = GetHeight();
	}

	vita2d_draw_texture_part_scale(m_pVitaTexture, pos.x, pos.y, trueSrcRect.left, trueSrcRect.bottom, trueSrcRect.width, trueSrcRect.height, scale.x, scale.y);
}

void Texture::DrawRotation(const Point2f& pos, const Rectf& srcRect, const Scale2f& scale, float rad) const
{
	Rectf trueSrcRect{ srcRect };
	// full texture
	if (srcRect.width == 0 || srcRect.height == 0)
	{
		trueSrcRect.width = GetWidth();
		trueSrcRect.height = GetHeight();
	}

	vita2d_draw_texture_part_scale_rotate(m_pVitaTexture, pos.x, pos.y, trueSrcRect.left, trueSrcRect.bottom, trueSrcRect.width, trueSrcRect.height, scale.x, scale.y, rad);
}

void Texture::DrawRotation(const Point2& pos, const Rectf& srcRect, const Scale2f& scale, float rad) const
{
	Rectf trueSrcRect{ srcRect };
	// full texture
	if (srcRect.width == 0 || srcRect.height == 0)
	{
		trueSrcRect.width = GetWidth();
		trueSrcRect.height = GetHeight();
	}

	vita2d_draw_texture_part_scale_rotate(m_pVitaTexture, pos.x, pos.y, trueSrcRect.left, trueSrcRect.bottom, trueSrcRect.width, trueSrcRect.height, scale.x, scale.y, rad);
}

float Texture::GetWidth() const
{
	return vita2d_texture_get_width(m_pVitaTexture);
}

float Texture::GetHeight() const
{
	return vita2d_texture_get_height(m_pVitaTexture);
}
