#version 120

uniform sampler2DRect tex0;
uniform sampler2DRect tex1;
uniform float alpha0;
uniform float alpha1;

varying vec2 texCoordVarying;

void main()
{
    vec4 c0 = texture2DRect(tex0, texCoordVarying);
    vec4 c1 = texture2DRect(tex1, texCoordVarying);
    
    vec4 color = vec4(1, 1, 1, 1);
    if(c0.r + c0.g + c0.b > 0.01){
        color = c0;
        color.a = alpha0;
    }else{
        color = c1;
        color.a = alpha1;
    }
    
    gl_FragColor = color;
}
