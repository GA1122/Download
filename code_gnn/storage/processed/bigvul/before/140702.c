bool GLES2DecoderImpl::CheckBoundReadFramebufferColorAttachment(
    const char* func_name) {
  Framebuffer* framebuffer = features().chromium_framebuffer_multisample ?
      framebuffer_state_.bound_read_framebuffer.get() :
      framebuffer_state_.bound_draw_framebuffer.get();
  if (!framebuffer)
    return true;
  if (framebuffer->GetAttachment(GL_COLOR_ATTACHMENT0) == NULL) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION, func_name, "no color image attached");
    return false;
  }
  return true;
}
