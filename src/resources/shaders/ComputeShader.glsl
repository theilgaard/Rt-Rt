#version 430 core

struct Sphere{
    vec3    pos;
	float	radius;
    vec3	color;
};

struct Ray {
    vec3    dir;
    vec3    origin;
};

//static point light
vec3 lightPos = vec3(2.5f, 2.5f, 2.5f); 
vec3 lightColor = vec3(1.0f);
float k_a = 0.1f;
float k_d = 0.6f;
float k_s = 0.3f;
float shininess = 20.0f; 

layout (local_size_x = 16, local_size_y = 16) in;

layout (rgba8, binding = 0) uniform image2D outputTex;
uniform float windowx; // number of pixels in the window on the x-axis
uniform float windowy; // number of pixels in the window on the y-axis
uniform mat4x3 C;

float focalDistance = 1.0f; // This probably needs to be ginven to the shader from "outside, and not be defined here"
float width = windowx/windowy; // The widht of the viewport. Not in pixels, but in "world size".
float height = 1.0f; // The height of the viewport. This should probably always be 1, and then x be scaled to fit non-square resolutions.
// the above 1-1-1 values should give an effective fov of 45 deg

vec4 bgColor = vec4(0.0f, 0.0f, 0.2f, 1.0f);

vec4 traceRay(Ray ray, Sphere s) {
	vec3 d = ray.dir;
	vec3 o = ray.origin;
	float r = s.radius;
	vec3 c = s.pos;
	float A = dot(d, d);
	float B = 2 * dot(o - c, d);
	float C = dot(o - c, o - c) - (r * r);

	float disc = B*B - 4*A*C;

	if(disc < 0.0f) {  // no hit
		return bgColor;
	}
	
	float t;
	if(disc == 0.0f) {  // one hit
		t = -B/(2.0f*A);
	} else {			// two hits
		float discSqrt = sqrt(disc);
		t = min((-B+discSqrt)/(2.0f*A),(-B-discSqrt)/(2.0f*A));
	}

	if(t <= 0) {        // we dont render backfaces (or insides of spheres) yet, so no t1<0, t2>0? stuff
		return bgColor;
	}
	
	vec3 h = o + t*d;
	vec3 normal = normalize(h - c);
	vec3 eyeDir = -d;

	// for each light...
	vec3 lightDir = normalize(lightPos - h);
	vec3 lightReflect = 2 * dot(lightDir, normal) * normal - lightDir;
	float NdotL = max(dot(normal, lightDir), 0.0f);
	float RdotVpow = pow(max(dot(lightReflect, eyeDir), 0.0f), shininess);
	
	return vec4(s.color*(k_a + k_d*NdotL*lightColor + k_s*RdotVpow*lightColor), 1.0);
}


void main(){
	float x = gl_GlobalInvocationID.x;
	float y = gl_GlobalInvocationID.y;

	float uu = ((x+0.5f)/windowx - 0.5f)*width;
	float vv = ((y+0.5f)/windowy - 0.5f)*height;
	Ray ray = Ray(normalize(-focalDistance*C[2] + uu*C[0] + vv*C[1]), C[3]);

	Sphere sph = Sphere(vec3(0.0f, 0.0f, -10.0f), 1.0f, vec3(0.4f, 0.8f, 0.1f));
	
	imageStore(outputTex, ivec2(x,y), traceRay(ray, sph));
}