/*
 * PrimitiveVertexShader.glsl
 * Copyright (C) 2015-2016 Pokedude
 *
 * Simple vertex shader to display primitives
 */


#version 330

// Input variables
layout(location = 0) in vec2 in_vertex;

// Uniform variables
uniform mat4 uni_mvp;


/* Specifies the vertex position */
void main()
{
    gl_Position = uni_mvp * vec4(in_vertex, 0.0, 1.0);
}
