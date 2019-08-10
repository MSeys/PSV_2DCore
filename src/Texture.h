#pragma once
#include "pch.h"

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

	void Draw(const Point2f& pos, const Rectf& srcRect = Rectf{ 0, 0, 0, 0 }, const Scale2f& scale = Scale2f{ 1, 1 }) const;
	void Draw(const Point2& pos = Point2{ 0, 0 }, const Rectf& srcRect = Rectf{ 0, 0, 0, 0 }, const Scale2f& scale = Scale2f{ 1, 1 }) const;

	void DrawRotation(const Point2f& pos, const Rectf& srcRect = Rectf{ 0, 0, 0, 0 }, const Scale2f& scale = Scale2f{ 1, 1 }, float rad = 0.f) const;
	void DrawRotation(const Point2& pos = Point2{ 0, 0 }, const Rectf& srcRect = Rectf{ 0, 0, 0, 0 }, const Scale2f& scale = Scale2f{ 1, 1 }, float rad = 0.f) const;

	float GetWidth() const;
	float GetHeight() const;
};