void WebGLRenderingContextBase::bufferData(GLenum target,
                                           int64_t size,
                                           GLenum usage) {
  if (isContextLost())
    return;
  BufferDataImpl(target, size, nullptr, usage);
}
