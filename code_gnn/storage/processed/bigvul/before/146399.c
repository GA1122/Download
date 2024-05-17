bool WebGLRenderingContextBase::ValidateBufferDataUsage(
    const char* function_name,
    GLenum usage) {
  switch (usage) {
    case GL_STREAM_DRAW:
    case GL_STATIC_DRAW:
    case GL_DYNAMIC_DRAW:
      return true;
    default:
      SynthesizeGLError(GL_INVALID_ENUM, function_name, "invalid usage");
      return false;
  }
}
