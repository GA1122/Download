WebGLId WebGraphicsContext3DCommandBufferImpl::createRenderbuffer() {
  GLuint o;
  gl_->GenRenderbuffers(1, &o);
  return o;
}
