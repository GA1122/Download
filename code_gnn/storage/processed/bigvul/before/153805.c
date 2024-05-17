void GLES2Implementation::ScheduleCALayerSharedStateCHROMIUM(
    GLfloat opacity,
    GLboolean is_clipped,
    const GLfloat* clip_rect,
    GLint sorting_context_id,
    const GLfloat* transform) {
  uint32_t shm_size = 20 * sizeof(GLfloat);
  ScopedTransferBufferPtr buffer(shm_size, helper_, transfer_buffer_);
  if (!buffer.valid() || buffer.size() < shm_size) {
    SetGLError(GL_OUT_OF_MEMORY, "GLES2::ScheduleCALayerSharedStateCHROMIUM",
               "out of memory");
    return;
  }
  GLfloat* mem = static_cast<GLfloat*>(buffer.address());
  memcpy(mem + 0, clip_rect, 4 * sizeof(GLfloat));
  memcpy(mem + 4, transform, 16 * sizeof(GLfloat));
  helper_->ScheduleCALayerSharedStateCHROMIUM(opacity, is_clipped,
                                              sorting_context_id,
                                              buffer.shm_id(), buffer.offset());
}
