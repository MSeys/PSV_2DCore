#pragma once
#include "pch.h"
#include "vita2d.h"

void draw_texture_part_scale_rotate_hotspot_generic(const vita2d_texture* texture, float x, float y,
	float tex_x, float tex_y, float tex_w, float tex_h, float x_scale, float y_scale, float rad, float center_x, float center_y);

void vita2d_draw_texture_part_scale_rotate_hotspot(const vita2d_texture* texture, float x, float y,
	float tex_x, float tex_y, float tex_w, float tex_h, float x_scale, float y_scale, float rad, float center_x, float center_y);
