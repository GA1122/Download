WebGLTexture* WebGLRenderingContextBase::createTexture() {
  if (isContextLost())
    return nullptr;
  return WebGLTexture::Create(this);
}
