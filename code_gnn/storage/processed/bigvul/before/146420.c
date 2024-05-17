bool WebGLRenderingContextBase::ValidateStencilOrDepthFunc(
    const char* function_name,
    GLenum func) {
  switch (func) {
    case GL_NEVER:
    case GL_LESS:
    case GL_LEQUAL:
    case GL_GREATER:
    case GL_GEQUAL:
    case GL_EQUAL:
    case GL_NOTEQUAL:
    case GL_ALWAYS:
      return true;
    default:
      SynthesizeGLError(GL_INVALID_ENUM, function_name, "invalid function");
      return false;
  }
}
