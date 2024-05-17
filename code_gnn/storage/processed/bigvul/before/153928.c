void GLES2DecoderImpl::DeleteFramebuffersHelper(
    GLsizei n,
    const volatile GLuint* client_ids) {
  for (GLsizei ii = 0; ii < n; ++ii) {
    GLuint client_id = client_ids[ii];
    Framebuffer* framebuffer = GetFramebuffer(client_id);
    if (framebuffer && !framebuffer->IsDeleted()) {
      if (framebuffer == framebuffer_state_.bound_draw_framebuffer.get()) {
        GLenum target = GetDrawFramebufferTarget();

        if (workarounds().unbind_attachments_on_bound_render_fbo_delete)
          framebuffer->DoUnbindGLAttachmentsForWorkaround(target);

        api()->glBindFramebufferEXTFn(target, GetBackbufferServiceId());
        state_.UpdateWindowRectanglesForBoundDrawFramebufferClientID(0);
        framebuffer_state_.bound_draw_framebuffer = nullptr;
        framebuffer_state_.clear_state_dirty = true;
      }
      if (framebuffer == framebuffer_state_.bound_read_framebuffer.get()) {
        framebuffer_state_.bound_read_framebuffer = nullptr;
        GLenum target = GetReadFramebufferTarget();
        api()->glBindFramebufferEXTFn(target, GetBackbufferServiceId());
      }
      OnFboChanged();
      RemoveFramebuffer(client_id);
    }
  }
}
