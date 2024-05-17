void WebGLRenderingContextBase::bufferData(GLenum target,
                                           MaybeShared<DOMArrayBufferView> data,
                                           GLenum usage) {
  if (isContextLost())
    return;
  DCHECK(data);
  BufferDataImpl(target, data.View()->byteLength(),
                 data.View()->BaseAddressMaybeShared(), usage);
}
