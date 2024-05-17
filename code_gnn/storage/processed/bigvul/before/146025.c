bool WebGL2RenderingContextBase::ValidateBufferDataUsage(
    const char* function_name,
    GLenum usage) {
  switch (usage) {
    case GL_STREAM_READ:
    case GL_STREAM_COPY:
    case GL_STATIC_READ:
    case GL_STATIC_COPY:
    case GL_DYNAMIC_READ:
    case GL_DYNAMIC_COPY:
      return true;
    default:
      return WebGLRenderingContextBase::ValidateBufferDataUsage(function_name,
                                                                usage);
  }
}
