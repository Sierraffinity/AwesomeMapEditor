/*
 * MapFragmentShader.glsl
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
uniform bool is_background;
uniform bool is_connection;


/* Maps pixel indices to colors */
void main()
{
    vec4 index = texture(smp_texture, frag_coords);
    vec4 texel = texture(smp_palette, index.xy);
    if (is_background == true)
    {
        texel.a = 1.0;
    }
    if (is_connection == true)
    {
        // Grayscaling connected maps for readability
        float rgb = (0.21*texel.r + 0.72*texel.g + 0.07*texel.b);
        texel.r = rgb;
        texel.g = rgb;
        texel.b = rgb;
    }

    // (Applies blending value for DNS implementations)
    // out_color = texel * vec4(frag_tint, 1.0);
    out_color = texel;
}
