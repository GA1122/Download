WebGLId WebGraphicsContext3DCommandBufferImpl::createTexture() {
  GLuint o;
  gl_->GenTextures(1, &o);
  return o;
}
