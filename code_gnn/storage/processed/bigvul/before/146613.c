void DrawingBuffer::ClearFramebuffersInternal(GLbitfield clear_mask) {
  DCHECK(state_restorer_);
  state_restorer_->SetFramebufferBindingDirty();
  if (multisample_fbo_) {
    gl_->BindFramebuffer(GL_FRAMEBUFFER, fbo_);
    gl_->Clear(GL_COLOR_BUFFER_BIT);
  }

  gl_->BindFramebuffer(GL_FRAMEBUFFER,
                       multisample_fbo_ ? multisample_fbo_ : fbo_);
  gl_->Clear(clear_mask);
}
