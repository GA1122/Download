WebGLId WebGraphicsContext3DCommandBufferImpl::createQueryEXT() {
  GLuint o;
  gl_->GenQueriesEXT(1, &o);
  return o;
}
