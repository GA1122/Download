void WebGL2RenderingContextBase::bufferSubData(
    GLenum target,
    GLintptr dst_byte_offset,
    MaybeShared<DOMArrayBufferView> src_data,
    GLuint src_offset,
    GLuint length) {
  if (isContextLost())
    return;
  void* sub_base_address = nullptr;
  long long sub_byte_length = 0;
  if (!ValidateSubSourceAndGetData(src_data.View(), src_offset, length,
                                   &sub_base_address, &sub_byte_length)) {
    SynthesizeGLError(GL_INVALID_VALUE, "bufferSubData",
                      "srcOffset + length too large");
    return;
  }
  BufferSubDataImpl(target, dst_byte_offset, sub_byte_length, sub_base_address);
}
