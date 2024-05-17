void GLES2DecoderImpl::DeleteTexturesHelper(
    GLsizei n, const GLuint* client_ids) {
  bool supports_separate_framebuffer_binds =
     features().chromium_framebuffer_multisample;
  for (GLsizei ii = 0; ii < n; ++ii) {
    TextureRef* texture_ref = GetTexture(client_ids[ii]);
    if (texture_ref) {
      Texture* texture = texture_ref->texture();
      if (texture->IsAttachedToFramebuffer()) {
        framebuffer_state_.clear_state_dirty = true;
      }
      for (size_t jj = 0; jj < state_.texture_units.size(); ++jj) {
        state_.texture_units[jj].Unbind(texture_ref);
      }
      if (supports_separate_framebuffer_binds) {
        if (framebuffer_state_.bound_read_framebuffer.get()) {
          framebuffer_state_.bound_read_framebuffer
              ->UnbindTexture(GL_READ_FRAMEBUFFER_EXT, texture_ref);
        }
        if (framebuffer_state_.bound_draw_framebuffer.get()) {
          framebuffer_state_.bound_draw_framebuffer
              ->UnbindTexture(GL_DRAW_FRAMEBUFFER_EXT, texture_ref);
        }
      } else {
        if (framebuffer_state_.bound_draw_framebuffer.get()) {
          framebuffer_state_.bound_draw_framebuffer
              ->UnbindTexture(GL_FRAMEBUFFER, texture_ref);
        }
      }
#if defined(OS_MACOSX)
      GLuint service_id = texture->service_id();
      if (texture->target() == GL_TEXTURE_RECTANGLE_ARB) {
        ReleaseIOSurfaceForTexture(service_id);
      }
#endif
      RemoveTexture(client_ids[ii]);
    }
  }
}
