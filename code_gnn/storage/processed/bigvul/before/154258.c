error::Error GLES2DecoderImpl::HandleMultiDrawArraysInstancedCHROMIUM(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const volatile gles2::cmds::MultiDrawArraysInstancedCHROMIUM& c =
      *static_cast<
          const volatile gles2::cmds::MultiDrawArraysInstancedCHROMIUM*>(
          cmd_data);
  if (!features().webgl_multi_draw_instanced) {
    return error::kUnknownCommand;
  }

  GLenum mode = static_cast<GLenum>(c.mode);
  GLsizei drawcount = static_cast<GLsizei>(c.drawcount);

  uint32_t firsts_size, counts_size, instance_counts_size;
  base::CheckedNumeric<uint32_t> checked_size(drawcount);
  if (!(checked_size * sizeof(GLint)).AssignIfValid(&firsts_size)) {
    return error::kOutOfBounds;
  }
  if (!(checked_size * sizeof(GLsizei)).AssignIfValid(&counts_size)) {
    return error::kOutOfBounds;
  }
  if (!(checked_size * sizeof(GLsizei)).AssignIfValid(&instance_counts_size)) {
    return error::kOutOfBounds;
  }
  const GLint* firsts = GetSharedMemoryAs<const GLint*>(
      c.firsts_shm_id, c.firsts_shm_offset, firsts_size);
  const GLsizei* counts = GetSharedMemoryAs<const GLsizei*>(
      c.counts_shm_id, c.counts_shm_offset, counts_size);
  const GLsizei* instance_counts = GetSharedMemoryAs<const GLsizei*>(
      c.instance_counts_shm_id, c.instance_counts_shm_offset,
      instance_counts_size);
  if (firsts == nullptr) {
    return error::kOutOfBounds;
  }
  if (counts == nullptr) {
    return error::kOutOfBounds;
  }
  if (instance_counts == nullptr) {
    return error::kOutOfBounds;
  }
  if (!multi_draw_manager_->MultiDrawArraysInstanced(
          mode, firsts, counts, instance_counts, drawcount)) {
    return error::kInvalidArguments;
  }
  return error::kNoError;
}
