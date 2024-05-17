bool GLES2DecoderImpl::WasContextLost() {
  if (context_->WasAllocatedUsingRobustnessExtension()) {
    GLenum status = GL_NO_ERROR;
    if (has_robustness_extension_)
      status = glGetGraphicsResetStatusARB();
    if (status != GL_NO_ERROR) {
      reset_status_ = status;
      LOG(ERROR) << (surface_->IsOffscreen() ? "Offscreen" : "Onscreen")
                 << " context lost via ARB/EXT_robustness. Reset status = 0x"
                 << std::hex << status << std::dec;
      return true;
    }
  }
  return false;
}
