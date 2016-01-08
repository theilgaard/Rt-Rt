#version 430 core

out vec2 texCoord;

vec4 vertices[4] = vec4[4](	vec4(-1.0f, -1.0f, 0.0f, 1.0f), vec4(-1.0f,  1.0f, 0.0f, 1.0f),
							vec4( 1.0f,  1.0f, 0.0f, 1.0f), vec4( 1.0f, -1.0f, 0.0f, 1.0f));
vec2 texCoords[4] = vec2[4](vec2( 0.0f,  0.0f), vec2( 0.0f,  1.0f),
							vec2( 1.0f,  1.0f), vec2( 1.0f,  0.0f));

void main()
{
	texCoord = texCoords[gl_VertexID];
	gl_Position = vertices[gl_VertexID];
}