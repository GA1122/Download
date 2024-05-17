void WebGLRenderingContextBase::bufferSubData(GLenum target,
                                              int64_t offset,
                                              DOMArrayBuffer* data) {
  if (isContextLost())
    return;
  DCHECK(data);
  BufferSubDataImpl(target, offset, data->ByteLength(), data->Data());
}
