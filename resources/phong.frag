#version 330

// Fragment shader

// These get passed in from the vertex shader and are interpolated (varying) properties,
// so they change for each pixel across the triangle:
in vec4 interpSurfPosition; // position on the surface in world coordinates
in vec3 interpSurfNormal; // normal in world coordinates
in vec2 texture_coordinate;


uniform vec4 lightPosition; // in world coordinates

//Textures
uniform sampler2D textureSampler;

uniform vec3 ambientReflectionCoeff;
uniform vec3 diffuseReflectionCoeff;
uniform vec3 specularReflectionCoeff;
uniform float shinyness;

uniform vec3 ambientLightIntensity;
uniform vec3 diffuseLightIntensity;
uniform vec3 specularLightIntensity;

// The camera location in world coordinates
uniform vec3 eye_world;

// This is an out variable for the final color we want to render this fragment.
out vec4 fragColor;

void main() {

    // Start with black and then add lighting to the final color as we calculate it
	vec3 materialColor = vec3(0.0, 0.0, 0.0);

	// TODO: Fill this in using to cook-torrance model
    vec3 N = normalize(interpSurfNormal);
    vec3 L = normalize(lightPosition - interpSurfPosition).xyz;
    vec3 E = normalize(eye_world - interpSurfPosition.xyz);
    float NdotL = clamp(dot(N, L), 0.0, 1.0);
    float EdotR = clamp(dot(E, normalize(reflect(-L, N))), 0.0, 1.0);
    
    float specularIntensity = pow(EdotR, shinyness);
    
    
    
    materialColor += ambientLightIntensity * ambientReflectionCoeff;
    materialColor += diffuseLightIntensity * diffuseReflectionCoeff * NdotL * texture(textureSampler, texture_coordinate).rgb;
    materialColor += specularLightIntensity * specularReflectionCoeff * specularIntensity;

    fragColor.rgb = materialColor.rgb;

	// And, set the alpha component to 1.0 (completely opaque, no transparency).
	fragColor.a = 1.0;
}
