#include "vita2d_extended.h"

void draw_texture_part_scale_rotate_hotspot_generic(const vita2d_texture* texture, float x, float y,
	float tex_x, float tex_y, float tex_w, float tex_h, float x_scale, float y_scale, float rad, float center_x, float center_y)
{
	vita2d_texture_vertex* vertices = (vita2d_texture_vertex*)vita2d_pool_memalign(
		4 * sizeof(vita2d_texture_vertex), // 4 vertices
		sizeof(vita2d_texture_vertex));

	const float center_x_scaled = x_scale * center_x;
	const float center_y_scaled = y_scale * center_y;
	
	const float w = vita2d_texture_get_width(texture);
	const float h = vita2d_texture_get_height(texture);

	const float u0 = tex_x / w;
	const float v0 = tex_y / h;
	const float u1 = (tex_x + tex_w) / w;
	const float v1 = (tex_y + tex_h) / h;

	tex_w *= x_scale;
	tex_h *= y_scale;

	vertices[0].x = -center_x_scaled;
	vertices[0].y = -center_y_scaled;
	vertices[0].z = +0.5f;
	vertices[0].u = u0;
	vertices[0].v = v0;

	vertices[1].x = tex_w - center_x_scaled;
	vertices[1].y = -center_y_scaled;
	vertices[1].z = +0.5f;
	vertices[1].u = u1;
	vertices[1].v = v0;

	vertices[2].x = -center_x_scaled;
	vertices[2].y = tex_h - center_y_scaled;
	vertices[2].z = +0.5f;
	vertices[2].u = u0;
	vertices[2].v = v1;

	vertices[3].x = tex_w - center_x_scaled;
	vertices[3].y = tex_h - center_y_scaled;
	vertices[3].z = +0.5f;
	vertices[3].u = u1;
	vertices[3].v = v1;

	const float c = cos(rad);
	const float s = sin(rad);
	int i;
	for (i = 0; i < 4; ++i) { // Rotate and translate
		float _x = vertices[i].x;
		float _y = vertices[i].y;
		vertices[i].x = _x * c - _y * s + x;
		vertices[i].y = _x * s + _y * c + y;
	}

	// Set the texture to the TEXUNIT0
	sceGxmSetFragmentTexture(_vita2d_context, 0, &texture->gxm_tex);

	sceGxmSetVertexStream(_vita2d_context, 0, vertices);
	sceGxmDraw(_vita2d_context, SCE_GXM_PRIMITIVE_TRIANGLE_STRIP, SCE_GXM_INDEX_FORMAT_U16, vita2d_get_linear_indices(), 4);
}

void vita2d_draw_texture_part_scale_rotate_hotspot(const vita2d_texture* texture, float x, float y,
	float tex_x, float tex_y, float tex_w, float tex_h, float x_scale, float y_scale, float rad, float center_x, float center_y)
{
	//set_texture_program();
	//set_texture_wvp_uniform();
	draw_texture_part_scale_rotate_hotspot_generic(texture, x, y,
		tex_x, tex_y, tex_w, tex_h, x_scale, y_scale, rad, center_x, center_y);
}