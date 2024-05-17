uint32_t GLES2Util::GLErrorToErrorBit(uint32_t error) {
  switch (error) {
    case GL_INVALID_ENUM:
      return gl_error_bit::kInvalidEnum;
    case GL_INVALID_VALUE:
      return gl_error_bit::kInvalidValue;
    case GL_INVALID_OPERATION:
      return gl_error_bit::kInvalidOperation;
    case GL_OUT_OF_MEMORY:
      return gl_error_bit::kOutOfMemory;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      return gl_error_bit::kInvalidFramebufferOperation;
    case GL_CONTEXT_LOST_KHR:
      return gl_error_bit::kContextLost;
    default:
      NOTREACHED();
      return gl_error_bit::kNoError;
  }
}
