/*
 * NormalFragmentShader.glsl
 * Copyright (C) 2015-2016 Pokedude
 *
 */


#version 330

// Input variables
in vec2 frag_coords;

// Output pixel color
out vec4 out_color;

// Uniform samplers
uniform sampler2D smp_texture;


/* Simply fetches ARGB pixel */
void main()
{
    /* Alters the transparency value (50%) */
    out_color = texture(smp_texture, frag_coords) * vec4(1.0, 1.0, 1.0, 0.5);
}
