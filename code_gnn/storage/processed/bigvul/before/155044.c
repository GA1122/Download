WebGLShaderPrecisionFormat* WebGLRenderingContextBase::getShaderPrecisionFormat(
    GLenum shader_type,
    GLenum precision_type) {
  if (isContextLost())
    return nullptr;
  if (!ValidateShaderType("getShaderPrecisionFormat", shader_type)) {
    return nullptr;
  }
  switch (precision_type) {
    case GL_LOW_FLOAT:
    case GL_MEDIUM_FLOAT:
    case GL_HIGH_FLOAT:
    case GL_LOW_INT:
    case GL_MEDIUM_INT:
    case GL_HIGH_INT:
      break;
    default:
      SynthesizeGLError(GL_INVALID_ENUM, "getShaderPrecisionFormat",
                        "invalid precision type");
      return nullptr;
  }

  GLint range[2] = {0, 0};
  GLint precision = 0;
  ContextGL()->GetShaderPrecisionFormat(shader_type, precision_type, range,
                                        &precision);
  return WebGLShaderPrecisionFormat::Create(range[0], range[1], precision);
}
