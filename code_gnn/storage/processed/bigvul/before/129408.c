error::Error GLES2DecoderImpl::HandleGenSharedIdsCHROMIUM(
    uint32 immediate_data_size, const cmds::GenSharedIdsCHROMIUM& c) {
  GLuint namespace_id = static_cast<GLuint>(c.namespace_id);
  GLuint id_offset = static_cast<GLuint>(c.id_offset);
  GLsizei n = static_cast<GLsizei>(c.n);
  uint32 data_size;
  if (!SafeMultiplyUint32(n, sizeof(GLuint), &data_size)) {
    return error::kOutOfBounds;
  }
  GLuint* ids = GetSharedMemoryAs<GLuint*>(
      c.ids_shm_id, c.ids_shm_offset, data_size);
  if (n < 0) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, "GenSharedIdsCHROMIUM", "n < 0");
    return error::kNoError;
  }
  if (ids == NULL) {
    return error::kOutOfBounds;
  }
  DoGenSharedIdsCHROMIUM(namespace_id, id_offset, n, ids);
  return error::kNoError;
}
