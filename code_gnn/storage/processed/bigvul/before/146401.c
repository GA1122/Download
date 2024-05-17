bool WebGLRenderingContextBase::ValidateCapability(const char* function_name,
                                                   GLenum cap) {
  switch (cap) {
    case GL_BLEND:
    case GL_CULL_FACE:
    case GL_DEPTH_TEST:
    case GL_DITHER:
    case GL_POLYGON_OFFSET_FILL:
    case GL_SAMPLE_ALPHA_TO_COVERAGE:
    case GL_SAMPLE_COVERAGE:
    case GL_SCISSOR_TEST:
    case GL_STENCIL_TEST:
      return true;
    default:
      SynthesizeGLError(GL_INVALID_ENUM, function_name, "invalid capability");
      return false;
  }
}
