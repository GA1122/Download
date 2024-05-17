void GLES2Implementation::ScheduleCALayerCHROMIUM(GLuint contents_texture_id,
                                                  const GLfloat* contents_rect,
                                                  GLuint background_color,
                                                  GLuint edge_aa_mask,
                                                  const GLfloat* bounds_rect,
                                                  GLuint filter) {
  uint32_t shm_size = 8 * sizeof(GLfloat);
  ScopedTransferBufferPtr buffer(shm_size, helper_, transfer_buffer_);
  if (!buffer.valid() || buffer.size() < shm_size) {
    SetGLError(GL_OUT_OF_MEMORY, "GLES2::ScheduleCALayerCHROMIUM",
               "out of memory");
    return;
  }
  GLfloat* mem = static_cast<GLfloat*>(buffer.address());
  memcpy(mem + 0, contents_rect, 4 * sizeof(GLfloat));
  memcpy(mem + 4, bounds_rect, 4 * sizeof(GLfloat));
  helper_->ScheduleCALayerCHROMIUM(contents_texture_id, background_color,
                                   edge_aa_mask, filter, buffer.shm_id(),
                                   buffer.offset());
}
