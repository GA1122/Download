void DrawingBuffer::AttachColorBufferToReadFramebuffer() {
  DCHECK(state_restorer_);
  state_restorer_->SetFramebufferBindingDirty();
  state_restorer_->SetTextureBindingDirty();

  gl_->BindFramebuffer(GL_FRAMEBUFFER, fbo_);

  GLenum target = back_color_buffer_->parameters.target;
  GLenum id = back_color_buffer_->texture_id;

  gl_->BindTexture(target, id);

  if (anti_aliasing_mode_ == kMSAAImplicitResolve)
    gl_->FramebufferTexture2DMultisampleEXT(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, target, id, 0, sample_count_);
  else
    gl_->FramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, target, id,
                              0);
}
