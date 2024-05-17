WebGLId WebGraphicsContext3DCommandBufferImpl::createBuffer() {
  GLuint o;
  gl_->GenBuffers(1, &o);
  return o;
}
