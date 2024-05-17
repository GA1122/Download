bool GLES2DecoderImpl::WasContextLost() {
  if (reset_status_ != GL_NO_ERROR) {
    return true;
  }
  if (context_->WasAllocatedUsingRobustnessExtension()) {
    GLenum status = GL_NO_ERROR;
    if (has_robustness_extension_)
      status = glGetGraphicsResetStatusARB();
    if (status != GL_NO_ERROR) {
      reset_status_ = status;
      reset_by_robustness_extension_ = true;
      LOG(ERROR) << (surface_->IsOffscreen() ? "Offscreen" : "Onscreen")
                 << " context lost via ARB/EXT_robustness. Reset status = "
                 << GLES2Util::GetStringEnum(status);
      return true;
    }
  }
  return false;
}
