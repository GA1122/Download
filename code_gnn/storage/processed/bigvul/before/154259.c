error::Error GLES2DecoderImpl::HandleMultiDrawElementsCHROMIUM(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const volatile gles2::cmds::MultiDrawElementsCHROMIUM& c =
      *static_cast<const volatile gles2::cmds::MultiDrawElementsCHROMIUM*>(
          cmd_data);
  if (!features().webgl_multi_draw) {
    return error::kUnknownCommand;
  }

  GLenum mode = static_cast<GLenum>(c.mode);
  GLenum type = static_cast<GLenum>(c.type);
  GLsizei drawcount = static_cast<GLsizei>(c.drawcount);

  uint32_t counts_size, offsets_size;
  base::CheckedNumeric<uint32_t> checked_size(drawcount);
  if (!(checked_size * sizeof(GLsizei)).AssignIfValid(&counts_size)) {
    return error::kOutOfBounds;
  }
  if (!(checked_size * sizeof(GLsizei)).AssignIfValid(&offsets_size)) {
    return error::kOutOfBounds;
  }
  const GLsizei* counts = GetSharedMemoryAs<const GLsizei*>(
      c.counts_shm_id, c.counts_shm_offset, counts_size);
  const GLsizei* offsets = GetSharedMemoryAs<const GLsizei*>(
      c.offsets_shm_id, c.offsets_shm_offset, offsets_size);
  if (counts == nullptr) {
    return error::kOutOfBounds;
  }
  if (offsets == nullptr) {
    return error::kOutOfBounds;
  }
  if (!multi_draw_manager_->MultiDrawElements(mode, counts, type, offsets,
                                              drawcount)) {
    return error::kInvalidArguments;
  }
  return error::kNoError;
}
