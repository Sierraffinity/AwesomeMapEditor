/*
 * PrimitiveFragmentShader.glsl
 * Copyright (C) 2015-2016 Pokedude
 *
 */


#version 330

// Output pixel color
out vec4 out_color;

// Uniform variables
uniform vec4 uni_color;


/* Copies uniform color value */
void main()
{
    out_color = uni_color;
}
