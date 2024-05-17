uint32_t GLES2Util::GLErrorBitToGLError(uint32_t error_bit) {
  switch (error_bit) {
    case gl_error_bit::kInvalidEnum:
      return GL_INVALID_ENUM;
    case gl_error_bit::kInvalidValue:
      return GL_INVALID_VALUE;
    case gl_error_bit::kInvalidOperation:
      return GL_INVALID_OPERATION;
    case gl_error_bit::kOutOfMemory:
      return GL_OUT_OF_MEMORY;
    case gl_error_bit::kInvalidFramebufferOperation:
      return GL_INVALID_FRAMEBUFFER_OPERATION;
    case gl_error_bit::kContextLost:
      return GL_CONTEXT_LOST_KHR;
    default:
      NOTREACHED();
      return GL_NO_ERROR;
  }
}
