bool WebGL2RenderingContextBase::ValidateClearBuffer(const char* function_name,
                                                     GLenum buffer,
                                                     GLsizei size,
                                                     GLuint src_offset) {
  CheckedNumeric<GLsizei> checked_size(size);
  checked_size -= src_offset;
  if (!checked_size.IsValid()) {
    SynthesizeGLError(GL_INVALID_VALUE, function_name,
                      "invalid array size / srcOffset");
    return false;
  }
  switch (buffer) {
    case GL_COLOR:
      if (checked_size.ValueOrDie() < 4) {
        SynthesizeGLError(GL_INVALID_VALUE, function_name,
                          "invalid array size / srcOffset");
        return false;
      }
      break;
    case GL_DEPTH:
    case GL_STENCIL:
      if (checked_size.ValueOrDie() < 1) {
        SynthesizeGLError(GL_INVALID_VALUE, function_name,
                          "invalid array size / srcOffset");
        return false;
      }
      break;
    default:
      SynthesizeGLError(GL_INVALID_ENUM, function_name, "invalid buffer");
      return false;
  }
  return true;
}
