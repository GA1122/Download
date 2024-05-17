void WebGLRenderingContextBase::bufferSubData(
    GLenum target,
    long long offset,
    const FlexibleArrayBufferView& data) {
  if (isContextLost())
    return;
  DCHECK(data);
  BufferSubDataImpl(target, offset, data.ByteLength(),
                    data.BaseAddressMaybeOnStack());
}
