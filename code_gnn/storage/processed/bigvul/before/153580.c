void GLES2Implementation::BufferSubDataHelper(GLenum target,
                                              GLintptr offset,
                                              GLsizeiptr size,
                                              const void* data) {
  if (size == 0) {
    return;
  }

  if (!ValidateSize("glBufferSubData", size) ||
      !ValidateOffset("glBufferSubData", offset)) {
    return;
  }

  GLuint buffer_id;
  if (GetBoundPixelTransferBuffer(target, "glBufferSubData", &buffer_id)) {
    if (!buffer_id) {
      return;
    }
    BufferTracker::Buffer* buffer = buffer_tracker_->GetBuffer(buffer_id);
    if (!buffer) {
      SetGLError(GL_INVALID_VALUE, "glBufferSubData", "unknown buffer");
      return;
    }

    int32_t end = 0;
    int32_t buffer_size = buffer->size();
    if (!base::CheckAdd(offset, size).AssignIfValid(&end) ||
        end > buffer_size) {
      SetGLError(GL_INVALID_VALUE, "glBufferSubData", "out of range");
      return;
    }

    if (buffer->address() && data)
      memcpy(static_cast<uint8_t*>(buffer->address()) + offset, data, size);
    return;
  }

  ScopedTransferBufferPtr buffer(size, helper_, transfer_buffer_);
  BufferSubDataHelperImpl(target, offset, size, data, &buffer);
}
