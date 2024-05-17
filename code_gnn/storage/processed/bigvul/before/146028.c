bool WebGL2RenderingContextBase::ValidateCapability(const char* function_name,
                                                    GLenum cap) {
  switch (cap) {
    case GL_RASTERIZER_DISCARD:
      return true;
    default:
      return WebGLRenderingContextBase::ValidateCapability(function_name, cap);
  }
}
