void WebGraphicsContext3DCommandBufferImpl::deleteBuffer(WebGLId buffer) {
  gl_->DeleteBuffers(1, &buffer);
}
