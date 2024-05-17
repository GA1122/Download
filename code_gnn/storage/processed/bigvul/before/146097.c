void WebGL2RenderingContextBase::getBufferSubData(
    GLenum target,
    long long src_byte_offset,
    MaybeShared<DOMArrayBufferView> dst_data,
    GLuint dst_offset,
    GLuint length) {
  WebGLBuffer* source_buffer = nullptr;
  void* destination_data_ptr = nullptr;
  long long destination_byte_length = 0;
  const char* message = ValidateGetBufferSubData(
      __FUNCTION__, target, src_byte_offset, dst_data.View(), dst_offset,
      length, &source_buffer, &destination_data_ptr, &destination_byte_length);
  if (message) {
    return;
  }

  if (!destination_byte_length) {
    return;
  }

  void* mapped_data = ContextGL()->MapBufferRange(
      target, static_cast<GLintptr>(src_byte_offset), destination_byte_length,
      GL_MAP_READ_BIT);

  if (!mapped_data)
    return;

  memcpy(destination_data_ptr, mapped_data, destination_byte_length);

  ContextGL()->UnmapBuffer(target);
}
