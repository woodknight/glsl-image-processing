#version 460 core

in vec2 TexCoord;
out vec4 FragColor;

// texture samlers
uniform sampler2D ytex;
uniform sampler2D uvtex;

void main()
{
    float r, g, b, y, u, v;
    
    y = texture(ytex, TexCoord).r - 16.0f/255.0f;
    v = texture(uvtex, TexCoord).r - 0.5f;
    u = texture(uvtex, TexCoord).g - 0.5f;

    r = 1.164 * y + 1.596 * v;
    g = 1.164 * y - 0.813 * v - 0.391 * u;
    b = 1.164 * y + 2.018 * u;

    FragColor = clamp(vec4(r, g, b, 1), 0.0, 1.0);
}