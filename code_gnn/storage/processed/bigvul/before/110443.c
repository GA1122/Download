void GLES2DecoderImpl::DeleteFramebuffersHelper(
    GLsizei n, const GLuint* client_ids) {
  bool supports_separate_framebuffer_binds =
     feature_info_->feature_flags().chromium_framebuffer_multisample;

  for (GLsizei ii = 0; ii < n; ++ii) {
    FramebufferManager::FramebufferInfo* framebuffer =
        GetFramebufferInfo(client_ids[ii]);
    if (framebuffer && !framebuffer->IsDeleted()) {
      if (framebuffer == bound_draw_framebuffer_) {
        bound_draw_framebuffer_ = NULL;
        state_dirty_ = true;
        GLenum target = supports_separate_framebuffer_binds ?
            GL_DRAW_FRAMEBUFFER_EXT : GL_FRAMEBUFFER;
        glBindFramebufferEXT(target, GetBackbufferServiceId());
      }
      if (framebuffer == bound_read_framebuffer_) {
        bound_read_framebuffer_ = NULL;
        GLenum target = supports_separate_framebuffer_binds ?
            GL_READ_FRAMEBUFFER_EXT : GL_FRAMEBUFFER;
        glBindFramebufferEXT(target, GetBackbufferServiceId());
      }
      RemoveFramebufferInfo(client_ids[ii]);
    }
  }
}
