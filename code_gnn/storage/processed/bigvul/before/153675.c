BufferTracker::Buffer* GLES2Implementation::GetBoundPixelTransferBufferIfValid(
    GLuint buffer_id,
    const char* function_name,
    GLuint offset,
    GLsizei size) {
  DCHECK(buffer_id);
  BufferTracker::Buffer* buffer = buffer_tracker_->GetBuffer(buffer_id);
  if (!buffer) {
    SetGLError(GL_INVALID_OPERATION, function_name, "invalid buffer");
    return nullptr;
  }
  if (buffer->mapped()) {
    SetGLError(GL_INVALID_OPERATION, function_name, "buffer mapped");
    return nullptr;
  }
  base::CheckedNumeric<uint32_t> buffer_offset = buffer->shm_offset();
  buffer_offset += offset;
  if (!buffer_offset.IsValid()) {
    SetGLError(GL_INVALID_VALUE, function_name, "offset to large");
    return nullptr;
  }
  base::CheckedNumeric<uint32_t> required_size = offset;
  required_size += size;
  if (!required_size.IsValid() ||
      buffer->size() < required_size.ValueOrDefault(0)) {
    SetGLError(GL_INVALID_VALUE, function_name, "unpack size to large");
    return nullptr;
  }
  return buffer;
}
