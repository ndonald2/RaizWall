
uniform sampler2D tex;

void main(){
    
    vec2 pos = gl_TexCoord[0].st;
    vec4 spriteColor = texture2D(tex,pos);
    
    // black level masks alpha
    //spriteColor.a = spriteColor.r;
    
    gl_FragColor = spriteColor * gl_Color;
}