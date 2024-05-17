void WebGLRenderingContextBase::bufferData(GLenum target,
                                           long long size,
                                           GLenum usage) {
  if (isContextLost())
    return;
  BufferDataImpl(target, size, 0, usage);
}
