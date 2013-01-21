
uniform sampler2DRect texSampler;
uniform float colorDecay;
uniform float alphaDecay;
uniform float alphaMin;

void main() {
    vec4 color = texture2DRect(texSampler, gl_TexCoord[0].xy);
    color.rgb *= colorDecay;
    color.a *= alphaDecay;
    gl_FragColor = color.a < alphaMin ? vec4(0,0,0,0) : color;
}