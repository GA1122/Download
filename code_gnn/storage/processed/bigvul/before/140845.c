void GLES2DecoderImpl::OnFboChanged() const {
  if (workarounds().restore_scissor_on_fbo_change)
    state_.fbo_binding_for_scissor_workaround_dirty_ = true;

  if (workarounds().gl_begin_gl_end_on_fbo_change_to_backbuffer) {
    GLint bound_fbo_unsigned = -1;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &bound_fbo_unsigned);
    GLuint bound_fbo = static_cast<GLuint>(bound_fbo_unsigned);
    if (surface_ && surface_->GetBackingFrameBufferObject() == bound_fbo)
      surface_->NotifyWasBound();
  }
}
