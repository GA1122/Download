void WebGLRenderingContextBase::bufferSubData(GLenum target,
                                              long long offset,
                                              DOMArrayBuffer* data) {
  if (isContextLost())
    return;
  DCHECK(data);
  BufferSubDataImpl(target, offset, data->ByteLength(), data->Data());
}
