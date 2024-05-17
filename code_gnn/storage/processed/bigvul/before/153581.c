void GLES2Implementation::BufferSubDataHelperImpl(
    GLenum target,
    GLintptr offset,
    GLsizeiptr size,
    const void* data,
    ScopedTransferBufferPtr* buffer) {
  DCHECK(buffer);
  DCHECK_GT(size, 0);

  auto DoBufferSubData = [&](const std::array<uint32_t, 1>&,
                             uint32_t copy_offset, uint32_t) {
    helper_->BufferSubData(target, offset + copy_offset, buffer->size(),
                           buffer->shm_id(), buffer->offset());
    InvalidateReadbackBufferShadowDataCHROMIUM(GetBoundBufferHelper(target));
  };

  if (!TransferArraysAndExecute(size, buffer, DoBufferSubData,
                                static_cast<const int8_t*>(data))) {
    SetGLError(GL_OUT_OF_MEMORY, "glBufferSubData", "out of memory");
  }
}
