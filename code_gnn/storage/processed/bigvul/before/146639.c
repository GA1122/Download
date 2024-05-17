void DrawingBuffer::ResolveAndBindForReadAndDraw() {
  {
    ScopedStateRestorer scoped_state_restorer(this);
    ResolveIfNeeded();
  }
  gl_->BindFramebuffer(GL_FRAMEBUFFER, fbo_);
}
