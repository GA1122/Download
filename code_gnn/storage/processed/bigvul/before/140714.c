void GLES2DecoderImpl::DeleteFramebuffersHelper(
    GLsizei n, const GLuint* client_ids) {
  bool supports_separate_framebuffer_binds =
     features().chromium_framebuffer_multisample;

  for (GLsizei ii = 0; ii < n; ++ii) {
    Framebuffer* framebuffer =
        GetFramebuffer(client_ids[ii]);
    if (framebuffer && !framebuffer->IsDeleted()) {
      if (framebuffer == framebuffer_state_.bound_draw_framebuffer.get()) {
        GLenum target = supports_separate_framebuffer_binds ?
            GL_DRAW_FRAMEBUFFER_EXT : GL_FRAMEBUFFER;

        if (workarounds().unbind_attachments_on_bound_render_fbo_delete)
          framebuffer->DoUnbindGLAttachmentsForWorkaround(target);

        glBindFramebufferEXT(target, GetBackbufferServiceId());
        framebuffer_state_.bound_draw_framebuffer = NULL;
        framebuffer_state_.clear_state_dirty = true;
      }
      if (framebuffer == framebuffer_state_.bound_read_framebuffer.get()) {
        framebuffer_state_.bound_read_framebuffer = NULL;
        GLenum target = supports_separate_framebuffer_binds ?
            GL_READ_FRAMEBUFFER_EXT : GL_FRAMEBUFFER;
        glBindFramebufferEXT(target, GetBackbufferServiceId());
      }
      OnFboChanged();
      RemoveFramebuffer(client_ids[ii]);
    }
  }
}
