/*
 * IndexedFragmentShader.glsl
 * Copyright (C) 2015-2016 Pokedude
 *
 */


#version 330

// Input variables
in vec2 frag_coords;
//in vec3 frag_tint;

// Output pixel color
out vec4 out_color;

// Uniform samplers
uniform sampler2D smp_palette;
uniform sampler2D smp_texture;


/* Maps pixel indices to colors */
void main()
{
    vec4 index = texture(smp_texture, frag_coords);
    vec4 texel = texture(smp_palette, index.xy);

    // (Applies blending value for DNS implementations)
    // out_color = texel * vec4(frag_tint, 1.0);
    out_color = texel;
}
