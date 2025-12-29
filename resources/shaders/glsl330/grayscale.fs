#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

// NOTE: Add your custom variables here

void main(){
    // Texel color fetching from texture sampler
    vec4 texelColor = texture(texture0, fragTexCoord)*colDiffuse*fragColor;

    // Convert texel color to grayscale using NTSC conversion weights
    float gray = dot(texelColor.rgb, vec3(0.299, 0.587, 0.114));

    // Mix original color with grayscale
    float factor = 0.5; // 0 = totalmente original, 1 = totalmente grayscale
    vec3 mixed = mix(texelColor.rgb, vec3(gray), factor);
    finalColor = vec4(mixed, texelColor.a);
}
