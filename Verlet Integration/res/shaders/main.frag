#version 330 core
struct Material
{
    sampler2D texture_diffuse0;
};

out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPosition;
in vec3 VertColor;
in vec3 Normal;

uniform vec3 cameraPosition;
uniform vec3 color;
uniform vec3 ambient;
uniform vec3 sunPosition;
uniform Material material;

float linearize_depth(float d,float zNear,float zFar)
{
    float z_n = 2.0 * d - 1.0;
    return 2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));
}

float fog_start = 0;
float fog_end = 100.0;

float e = 2.71828;
float density = 7.5;

void main()
{
    // im pretty sure this isn't right but whatever
    float depth = (fog_start + linearize_depth(gl_FragCoord.z, 0.1, 100.0)) / (fog_end - fog_start);
    float fog = (1 / pow(e, pow(depth * density, 2)));

    vec4 textureSample = texture(material.texture_diffuse0, TexCoords) * vec4(VertColor, 1.0);
    vec4 rawDiffuse = mix(textureSample, vec4(color, 1.0), 1 - textureSample.w);

    vec3 DirectionToSun = normalize(sunPosition - FragPosition);
    float diff = dot(Normal, DirectionToSun) * 0.5 + 0.5; // max(dot(Normal, DirectionToSun), 0.25)
    vec4 shaded = vec4(diff, diff, diff, 1.0);

    vec4 fragColor = rawDiffuse * fog * (vec4(vec3(1.0, 1.0, 1.0) + ambient, 1.0) * shaded);
    fragColor = clamp(fragColor, vec4(0.0, 0.0, 0.0, 0.0), vec4(1.0, 1.0, 1.0, 1.0));

    FragColor = fragColor;
}
