void GLES2DecoderImpl::DeleteRenderbuffersHelper(
    GLsizei n, const GLuint* client_ids) {
  bool supports_separate_framebuffer_binds =
     features().chromium_framebuffer_multisample;
  for (GLsizei ii = 0; ii < n; ++ii) {
    Renderbuffer* renderbuffer =
        GetRenderbuffer(client_ids[ii]);
    if (renderbuffer && !renderbuffer->IsDeleted()) {
      if (state_.bound_renderbuffer.get() == renderbuffer) {
        state_.bound_renderbuffer = NULL;
      }
      if (supports_separate_framebuffer_binds) {
        if (framebuffer_state_.bound_read_framebuffer.get()) {
          framebuffer_state_.bound_read_framebuffer
              ->UnbindRenderbuffer(GL_READ_FRAMEBUFFER_EXT, renderbuffer);
        }
        if (framebuffer_state_.bound_draw_framebuffer.get()) {
          framebuffer_state_.bound_draw_framebuffer
              ->UnbindRenderbuffer(GL_DRAW_FRAMEBUFFER_EXT, renderbuffer);
        }
      } else {
        if (framebuffer_state_.bound_draw_framebuffer.get()) {
          framebuffer_state_.bound_draw_framebuffer
              ->UnbindRenderbuffer(GL_FRAMEBUFFER, renderbuffer);
        }
      }
      framebuffer_state_.clear_state_dirty = true;
      RemoveRenderbuffer(client_ids[ii]);
    }
  }
}
