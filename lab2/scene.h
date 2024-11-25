#ifndef SCENE_H
#define SCENE_H
#include "vec3.h"

#define char_fig_rect(cx, cy, w, h, rot, r, g, b ) (char_fig) { .type = 'r', .c_pos = {cx, cy, 0}, .color = { .x = r, .y = g, .z = b}, .data = {w, h, rot, 0}}
#define char_fig_circle(cx, cy, rad, r ,g ,b) (char_fig) {.type = 'c', .c_pos = {cx, cy, 0}, .color = {.x = r, .y = g, .z = b}, .data = {rad, 0, 0, 0}}

#define BROWNR 84.f/255
#define BROWNG 34.f/255
#define BROWNB 27.f/255
#define GREENR 44.f/255
#define GREENG 156.f/255
#define GREENB 55.f/255

// type = 'c' - circle	=> data[0] radius
// type = 'r' - rect	=> data[0] w, data[1] h, data[2] angle (radians)
typedef struct
{
	char type;
	vec3f c_pos;
	vec3f color;
	float data[4];
} char_fig;

/*
		(-1.0,+1.0)		( 0.0,+1.0)		(+1.0,+1.0)
			+----------------+---------------+
			| A              B            C  |
			|  +--------------------------+  |
			|  |##########################|  |
			|  |##########################|  |
			|  |##########################|  |
(-1.0, 0.0) +--*##########################*--+ (+1.0, 0.0)
			|  |##########################|  |
			|  |##########################|  |
			|  |##########################|  |
			|  +--------------------------+  |
			|  F             E            D  |
			+----------------+---------------+
		(-1.0,-1.0)		( 0.0,-1.0)		(+1.0,-1.0)
	A (-0.8,  0.8 )
	B ( 0.0,  0.8 )
	C (+0.8,  0.8 )
	D (+0.8, -0.8 )
	E ( 0.0, -0.8 )
	F (-0.8, -0.8 )
*/

const char_fig scene[] = {
	char_fig_rect(0, 0, 1.6f, 1.f, 0, GREENR, GREENG, GREENB),

	char_fig_rect( 0.0f,  0.8f, 1.6f, 0.1f, 0, BROWNR, BROWNG, BROWNB),
	char_fig_rect( 0.0f, -0.8f, 1.6f, 0.1f, 0, BROWNR, BROWNG, BROWNB),
	char_fig_rect( 0.8f,  0.0f, 0.1f, 1.0f, 0, BROWNR, BROWNG, BROWNB),
	char_fig_rect(-0.8f,  0.0f, 0.1f, 1.0f, 0, BROWNR, BROWNG, BROWNB),

	char_fig_circle(-0.8f,  0.8f, 0.05f * 1.41f, 0, 0, 0),
	char_fig_circle( 0.0f,  0.8f, 0.05f        , 0, 0, 0),
	char_fig_circle(+0.8f,  0.8f, 0.05f * 1.41f, 0, 0, 0),
	char_fig_circle(+0.8f, -0.8f, 0.05f * 1.41f, 0, 0, 0),
	char_fig_circle( 0.0f, -0.8f, 0.05f        , 0, 0, 0),
	char_fig_circle(-0.8f, -0.8f, 0.05f * 1.41f, 0, 0, 0),
	
	char_fig_circle(-0.25f, +0.00f, 0.02f, 1.0f, 1.0f, 1.0f),

	char_fig_circle(+0.50f, +0.00f, 0.02f, 0.8f, 0.6f, 0.6f),
	char_fig_circle(+0.54f, +0.04f, 0.02f, 1.0f, 0.8f, 0.6f),
	char_fig_circle(+0.54f, -0.04f, 0.02f, 0.6f, 1.0f, 0.6f),
	char_fig_circle(+0.58f, +0.08f, 0.02f, 0.8f, 0.8f, 1.0f),
	char_fig_circle(+0.58f, +0.00f, 0.02f, 1.0f, 0.7f, 0.7f),
	char_fig_circle(+0.58f, -0.08f, 0.02f, 1.0f, 0.6f, 1.0f),
	char_fig_circle(+0.62f, +0.12f, 0.02f, 0.4f, 1.0f, 0.4f),
	char_fig_circle(+0.62f, +0.04f, 0.02f, 1.0f, 0.4f, 0.4f),
	char_fig_circle(+0.62f, -0.04f, 0.02f, 0.2f, 0.2f, 0.2f),
	char_fig_circle(+0.62f, -0.12f, 0.02f, 1.0f, 1.0f, 0.6f),
	char_fig_circle(+0.66f, +0.16f, 0.02f, 0.5f, 0.7f, 0.3f),
	char_fig_circle(+0.66f, +0.08f, 0.02f, 0.3f, 0.7f, 0.3f),
	char_fig_circle(+0.66f, +0.00f, 0.02f, 0.6f, 0.2f, 0.2f),
	char_fig_circle(+0.66f, -0.08f, 0.02f, 0.6f, 0.2f, 0.6f),
	char_fig_circle(+0.66f, -0.16f, 0.02f, 0.5f, 0.6f, 0.3f),

	char_fig_rect( -0.65f,  0.0f, 0.7f, 0.01f, 0, 0.7f, 0.6f, 0.5f),
};

#endif