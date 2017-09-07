#version 150

uniform sampler2DRect tex0;
uniform sampler2DRect tex1;
uniform float alpha0;
uniform float alpha1;

in vec2 texCoordVarying;

out vec4 outputColor;

void main()
{
    vec4 c0 = texture(tex0, texCoordVarying);
    vec4 c1 = texture(tex1, texCoordVarying);
    
    vec4 color = vec4(1, 1, 1, 1);
    if(c0.r + c0.g + c0.b > 0.01){
        color = c0;
        color.a = alpha0;
    }else{
        color = c1;
        color.a = alpha1;
    }
    
    outputColor = color;
}
