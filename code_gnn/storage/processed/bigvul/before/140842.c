void GLES2DecoderImpl::LoseContext(uint32 reset_status) {
  if (reset_status_ != GL_NO_ERROR) {
    return;
  }

  if (workarounds().use_virtualized_gl_contexts) {
    if (reset_status == GL_GUILTY_CONTEXT_RESET_ARB) {
      reset_status = GL_UNKNOWN_CONTEXT_RESET_ARB;
    }
  } else if (reset_status == GL_UNKNOWN_CONTEXT_RESET_ARB &&
             IsRobustnessSupported()) {
    GLenum driver_status = glGetGraphicsResetStatusARB();
    if (driver_status == GL_GUILTY_CONTEXT_RESET_ARB ||
        driver_status == GL_INNOCENT_CONTEXT_RESET_ARB) {
      reset_status = driver_status;
    }
  }

  reset_status_ = reset_status;
  current_decoder_error_ = error::kLostContext;
}
