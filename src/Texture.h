#pragma once
#include <vita2d.h>

class Texture
{
	vita2d_texture* m_pVitaTexture;

public:
	Texture(const std::string& fileName);

	Texture(const Texture& other) = delete;
	Texture& operator=(const Texture& rhs) = delete;
	Texture(const Texture&& other) = delete;
	Texture& operator=(const Texture&& rhs) = delete;

	~Texture();

	void Draw(const Point2f& pos = Point2f{ 0, 0 }, 
				const Rectf& srcRect = Rectf{ 0, 0, 0, 0 }, 
				const Scale2f& scales = Scale2f{ 1, 1 }, 
				float angle = 0.f, const Point2f& hotspot = Point2f{ -1, -1 }) const;

	void Draw(const Point2f& pos,
		float angle, const Point2f & hotspot = Point2f{ -1, -1 },
		const Rectf& srcRect = Rectf{ 0, 0, 0, 0 },
		const Scale2f& scales = Scale2f{ 1, 1 }) const;

	float GetWidth() const;
	float GetHeight() const;
};