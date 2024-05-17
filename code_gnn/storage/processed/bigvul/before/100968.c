void TextureMapperGL::initializeShaders()
{
    static bool shadersCompiled = false;
    if (shadersCompiled)
        return;
    shadersCompiled = true;
#ifndef TEXMAP_OPENGL_ES_2
#define OES2_PRECISION_DEFINITIONS \
    "#define lowp\n#define highp\n"
#else
#define OES2_PRECISION_DEFINITIONS
#endif

    const char* fragmentShaderSourceOpacityAndMask =
            OES2_PRECISION_DEFINITIONS
"               uniform sampler2D SourceTexture, MaskTexture;                       \n"
"               uniform lowp float Opacity;                                         \n"
"               varying highp vec2 OutTexCoordSource, OutTexCoordMask;              \n"
"               void main(void)                                                     \n"
"               {                                                                   \n"
"                   lowp vec4 color = texture2D(SourceTexture, OutTexCoordSource);  \n"
"                   lowp vec4 maskColor = texture2D(MaskTexture, OutTexCoordMask);  \n"
"                   lowp float o = Opacity * maskColor.a;                           \n"
"                   gl_FragColor = vec4(color.rgb * o, color.a * o);                \n"
"               }                                                                   \n";

    const char* vertexShaderSourceOpacityAndMask =
            OES2_PRECISION_DEFINITIONS
"               uniform mat4 InMatrix, InSourceMatrix, InMaskMatrix;            \n"
"               attribute vec4 InVertex;                                        \n"
"               varying highp vec2 OutTexCoordSource, OutTexCoordMask;          \n"
"               void main(void)                                                 \n"
"               {                                                               \n"
"                   OutTexCoordSource = vec2(InSourceMatrix * InVertex);        \n"
"                   OutTexCoordMask = vec2(InMaskMatrix * InVertex);            \n"
"                   gl_Position = InMatrix * InVertex;                          \n"
"               }                                                               \n";

    const char* fragmentShaderSourceSimple =
            OES2_PRECISION_DEFINITIONS
"               uniform sampler2D SourceTexture;                                    \n"
"               uniform lowp float Opacity;                                         \n"
"               varying highp vec2 OutTexCoordSource;                               \n"
"               void main(void)                                                     \n"
"               {                                                                   \n"
"                   lowp vec4 color = texture2D(SourceTexture, OutTexCoordSource);  \n"
"                   gl_FragColor = vec4(color.rgb * Opacity, color.a * Opacity);    \n"
"               }                                                                   \n";

    const char* vertexShaderSourceSimple =
            OES2_PRECISION_DEFINITIONS
"               uniform mat4 InMatrix, InSourceMatrix;                      \n"
"               attribute vec4 InVertex;                                    \n"
"               varying highp vec2 OutTexCoordSource;                       \n"
"               void main(void)                                             \n"
"               {                                                           \n"
"                   OutTexCoordSource = vec2(InSourceMatrix * InVertex);    \n"
"                   gl_Position = InMatrix * InVertex;                      \n"
"               }                                                           \n";

    const char* fragmentShaderSourceClip =
"               void main(void) { gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0); }                                ";

    const char* vertexShaderSourceClip =
"               uniform mat4 InMatrix;                                      \n"
"               attribute vec4 InVertex;                                    \n"
"               void main(void) { gl_Position = InMatrix * InVertex; }      ";


    TEXMAP_BUILD_SHADER(Simple)
    TEXMAP_BUILD_SHADER(OpacityAndMask)
    TEXMAP_BUILD_SHADER(Clip)

    TEXMAP_GET_SHADER_VAR_LOCATION(OpacityAndMask, InMatrix)
    TEXMAP_GET_SHADER_VAR_LOCATION(OpacityAndMask, InSourceMatrix)
    TEXMAP_GET_SHADER_VAR_LOCATION(OpacityAndMask, InMaskMatrix)
    TEXMAP_GET_SHADER_VAR_LOCATION(OpacityAndMask, SourceTexture)
    TEXMAP_GET_SHADER_VAR_LOCATION(OpacityAndMask, MaskTexture)
    TEXMAP_GET_SHADER_VAR_LOCATION(OpacityAndMask, Opacity)

    TEXMAP_GET_SHADER_VAR_LOCATION(Simple, InSourceMatrix)
    TEXMAP_GET_SHADER_VAR_LOCATION(Simple, InMatrix)
    TEXMAP_GET_SHADER_VAR_LOCATION(Simple, SourceTexture)
    TEXMAP_GET_SHADER_VAR_LOCATION(Simple, Opacity)

    TEXMAP_GET_SHADER_VAR_LOCATION(Clip, InMatrix)
}
