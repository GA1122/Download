bool GLES2DecoderImpl::CheckFramebufferComplete(const char* func_name) {
  if (bound_draw_framebuffer_ && bound_draw_framebuffer_->IsNotComplete()) {
    SetGLError(GL_INVALID_FRAMEBUFFER_OPERATION,
               (std::string(func_name) + " framebuffer incomplete").c_str());
    return false;
  }
  return true;
}
