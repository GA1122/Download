error::Error GLES2DecoderImpl::HandleBufferData(uint32_t immediate_data_size,
                                                const volatile void* cmd_data) {
  const volatile gles2::cmds::BufferData& c =
      *static_cast<const volatile gles2::cmds::BufferData*>(cmd_data);
  GLenum target = static_cast<GLenum>(c.target);
  GLsizeiptr size = static_cast<GLsizeiptr>(c.size);
  uint32_t data_shm_id = static_cast<uint32_t>(c.data_shm_id);
  uint32_t data_shm_offset = static_cast<uint32_t>(c.data_shm_offset);
  GLenum usage = static_cast<GLenum>(c.usage);
  const void* data = nullptr;
  if (data_shm_id != 0 || data_shm_offset != 0) {
    data = GetSharedMemoryAs<const void*>(data_shm_id, data_shm_offset, size);
    if (!data) {
      return error::kOutOfBounds;
    }
  }
  buffer_manager()->ValidateAndDoBufferData(&state_, error_state_.get(), target,
                                            size, data, usage);
  return error::kNoError;
}
