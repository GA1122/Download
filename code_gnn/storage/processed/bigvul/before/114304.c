void WebGraphicsContext3DCommandBufferImpl::deleteTexture(WebGLId texture) {
  gl_->DeleteTextures(1, &texture);
}
