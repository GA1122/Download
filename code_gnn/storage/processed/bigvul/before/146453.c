void WebGLRenderingContextBase::bufferData(GLenum target,
                                           DOMArrayBuffer* data,
                                           GLenum usage) {
  if (isContextLost())
    return;
  if (!data) {
    SynthesizeGLError(GL_INVALID_VALUE, "bufferData", "no data");
    return;
  }
  BufferDataImpl(target, data->ByteLength(), data->Data(), usage);
}
