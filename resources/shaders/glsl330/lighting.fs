#version 330

// Input vertex attributes (from vertex shader)
in vec3 fragPosition;
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

// NOTE: Add your custom variables here

#define     MAX_LIGHTS              4
#define     LIGHT_DIRECTIONAL       0
#define     LIGHT_POINT             1

struct Light {
    int enabled;
    int type;
    vec3 position;
    vec3 target;
    vec4 color;
};

// Input lighting values
uniform Light lights[MAX_LIGHTS];
uniform vec4 ambient;
uniform vec3 viewPos;

void main()
{
    // Texel color fetching from texture sampler
    vec4 texelColor = texture(texture0, fragTexCoord);
    vec3 lightDot = vec3(0.0);
    vec3 normal = normalize(fragNormal);
    vec3 viewD = normalize(viewPos - fragPosition);
    vec3 specular = vec3(0.0);

    vec4 tint = colDiffuse*fragColor;
    
    // NOTE: Implement here your fragment shader code

    for (int i = 0; i < MAX_LIGHTS; i++){
        if (lights[i].enabled == 1)
        {
            vec3 lightDir = vec3(0.0);

            if (lights[i].type == LIGHT_DIRECTIONAL)
            {
                lightDir = -normalize(lights[i].target - lights[i].position);
            }
            else if (lights[i].type == LIGHT_POINT)
            {
                lightDir = normalize(lights[i].position - fragPosition);
            }

            float NdotL = max(dot(normal, lightDir), 0.0);

            // ✅ atenuação por distância (AQUI)
            float distance = length(lights[i].position - fragPosition);
            float attenuation = 1.0 / (1.0 + 0.1*distance + 0.01*distance*distance);

            lightDot += lights[i].color.rgb * NdotL * attenuation;

            // especular
            if (NdotL > 0.0)
            {
                float specCo = pow(
                    max(dot(viewD, reflect(-lightDir, normal)), 0.0),
                    16.0
                );
                specular += specCo * attenuation;
            }
        }
    }



    vec3 baseColor = tint.rgb;

    vec3 diffuse = baseColor * lightDot;
    vec3 final = diffuse + specular + ambient.rgb * baseColor;

    finalColor = vec4(final, 1.0);


}