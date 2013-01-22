
attribute float particleSize;

void main() {
    gl_FrontColor = gl_Color;
    gl_PointSize  = particleSize;
    gl_Position  =  gl_ModelViewProjectionMatrix * gl_Vertex;
} 