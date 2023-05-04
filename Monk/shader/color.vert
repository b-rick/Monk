#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightingModel;

out vec3 oNormal;
out vec3 oFragPos;
out vec3 oLightPos;
out vec2 TexCoords;

void main()
{
	oFragPos = vec3(model * vec4(aPos, 1.0));
	gl_Position = projection * view * vec4(oFragPos, 1.0);

	// we multiply by the inverse transpose  model matrix to preserver the direction of normals under the model matrix
	// when subject to a non orthogonal model matrix. If model is orthogonal then we don't need to do this
	
	// this includes uniform scale matrices too
	oNormal = mat3(transpose(inverse(model))) * aNormal; 
	TexCoords = aTexCoords;
}