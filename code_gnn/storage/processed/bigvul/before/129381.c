static void GetShaderPrecisionFormatImpl(GLenum shader_type,
                                         GLenum precision_type,
                                         GLint *range, GLint *precision) {
  switch (precision_type) {
    case GL_LOW_INT:
    case GL_MEDIUM_INT:
    case GL_HIGH_INT:
      range[0] = 31;
      range[1] = 30;
      *precision = 0;
      break;
    case GL_LOW_FLOAT:
    case GL_MEDIUM_FLOAT:
    case GL_HIGH_FLOAT:
      range[0] = 127;
      range[1] = 127;
      *precision = 23;
      break;
    default:
      NOTREACHED();
      break;
  }

  if (gfx::GetGLImplementation() == gfx::kGLImplementationEGLGLES2 &&
      gfx::g_driver_gl.fn.glGetShaderPrecisionFormatFn) {
    glGetShaderPrecisionFormat(shader_type, precision_type,
                               range, precision);


    range[0] = abs(range[0]);
    range[1] = abs(range[1]);

    if (precision_type == GL_HIGH_FLOAT &&
        !PrecisionMeetsSpecForHighpFloat(range[0], range[1], *precision)) {
      range[0] = 0;
      range[1] = 0;
      *precision = 0;
    }
  }
}
