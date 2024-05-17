void GLES2DecoderImpl::RestoreFramebufferBindings() const {
  GLuint service_id =
      framebuffer_state_.bound_draw_framebuffer.get()
          ? framebuffer_state_.bound_draw_framebuffer->service_id()
          : GetBackbufferServiceId();
  if (!SupportsSeparateFramebufferBinds()) {
    api()->glBindFramebufferEXTFn(GL_FRAMEBUFFER, service_id);
  } else {
    api()->glBindFramebufferEXTFn(GL_DRAW_FRAMEBUFFER, service_id);
    service_id = framebuffer_state_.bound_read_framebuffer.get()
                     ? framebuffer_state_.bound_read_framebuffer->service_id()
                     : GetBackbufferServiceId();
    api()->glBindFramebufferEXTFn(GL_READ_FRAMEBUFFER, service_id);
  }
  OnFboChanged();
}
