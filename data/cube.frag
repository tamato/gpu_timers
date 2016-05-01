#version 430
#extension GL_ARB_shading_language_420pack: enable  // Use for GLSL versions before 420.

layout(location = 0) in vec3 Normal;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 EyeVector; // position - camera (going in the neg Z)
layout(location = 3) in vec3 LightDir;

layout(binding=0) uniform sampler2D NormalMap;

layout(location = 1) uniform vec4 Diffuse;
layout(location = 2) uniform float Shininess;

layout(location = 0) out vec4 FragColor;


// http://www.thetenthplanet.de/archives/1180
mat3 cotangent_frame(vec3 N, vec3 p, vec2 uv)
{
    // get edge vectors of the pixel triangle
    vec3 dp1 = dFdx( p );
    vec3 dp2 = dFdy( p );
    vec2 duv1 = dFdx( uv );
    vec2 duv2 = dFdy( uv );
 
    // solve the linear system
    vec3 dp2perp = cross( dp2, N );
    vec3 dp1perp = cross( N, dp1 );
    vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
    vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;
 
    // construct a scale-invariant frame 
    float invmax = inversesqrt( max( dot(T,T), dot(B,B) ) );
    return mat3( T * invmax, B * invmax, N );
}

vec3 perturb_normal( vec3 N, vec3 V, vec2 texcoord )
{
    // assume N, the interpolated vertex normal and 
    // V, the view vector (vertex to eye)
    vec3 map = texture( NormalMap, texcoord ).xyz;
    mat3 TBN = cotangent_frame( N, V, texcoord );
    map = 2 * map - 1;
    map.z = .7;
    map = normalize(map);
    return normalize( TBN * map );
}

void main() {
#if 1
    vec3 N = perturb_normal(Normal, EyeVector, TexCoord);
    vec3 E = -normalize(EyeVector);
    vec3 L = LightDir;
    vec3 H = normalize(E + L);
#else
    vec3 N = texture( NormalMap, TexCoord ).xyz;
    vec3 dpx = dFdx(EyeVector);
    vec3 dpy = dFdy(EyeVector);
    vec3 T = normalize(cross(dpy, Normal));
    vec3 B = normalize(cross(Normal, dpx));

    mat3 TBN = mat3(T,B,Normal);

    vec3 E = -normalize(EyeVector);
    vec3 L = LightDir;
    vec3 H = normalize(E + L);

    N = 2 * N - 1;
    N = TBN * N;
#endif

    float df = max(0, dot(L, N));
    float sf = max(0, dot(H, N));
    sf = pow(sf, Shininess);

    // check for self shadowing, the 8 is a number found from trail and error
    // http://developer.download.nvidia.com/assets/gamedev/docs/GDC2K_gpu_bump.pdf
    float ssdf = min(8 * max(0, dot(Normal, L)), 1);
    float sssf = min(8 * max(0, dot(Normal, H)), 1);

    FragColor = (ssdf * df * Diffuse) + (sssf * sf * vec4(1,1,1,1));
}