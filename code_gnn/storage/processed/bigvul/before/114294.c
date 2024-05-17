WebGLId WebGraphicsContext3DCommandBufferImpl::createFramebuffer() {
  GLuint o = 0;
  gl_->GenFramebuffers(1, &o);
  return o;
}
