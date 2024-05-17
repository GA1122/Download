bool GLES2DecoderImpl::WasContextLost() {
  if (context_->WasAllocatedUsingARBRobustness() && has_arb_robustness_) {
    GLenum status = glGetGraphicsResetStatusARB();
    if (status != GL_NO_ERROR) {
      reset_status_ = status;
      LOG(ERROR) << (surface_->IsOffscreen() ? "Offscreen" : "Onscreen")
                 << " context lost via ARB_robustness. Reset status = 0x"
                 << std::hex << status << std::dec;
      return true;
    }
  }
  return false;
}
