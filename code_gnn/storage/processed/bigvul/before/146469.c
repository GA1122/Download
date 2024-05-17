WebGLBuffer* WebGLRenderingContextBase::createBuffer() {
  if (isContextLost())
    return nullptr;
  return WebGLBuffer::Create(this);
}
