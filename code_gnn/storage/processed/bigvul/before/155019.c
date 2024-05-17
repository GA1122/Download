void WebGLRenderingContextBase::bufferSubData(
    GLenum target,
    int64_t offset,
    const FlexibleArrayBufferView& data) {
  if (isContextLost())
    return;
  DCHECK(data);
  BufferSubDataImpl(target, offset, data.ByteLength(),
                    data.BaseAddressMaybeOnStack());
}
