error::Error GLES2DecoderImpl::HandleBufferData(uint32 immediate_data_size,
                                                const void* cmd_data) {
  const gles2::cmds::BufferData& c =
      *static_cast<const gles2::cmds::BufferData*>(cmd_data);
  GLenum target = static_cast<GLenum>(c.target);
  GLsizeiptr size = static_cast<GLsizeiptr>(c.size);
  uint32 data_shm_id = static_cast<uint32>(c.data_shm_id);
  uint32 data_shm_offset = static_cast<uint32>(c.data_shm_offset);
  GLenum usage = static_cast<GLenum>(c.usage);
  const void* data = NULL;
  if (data_shm_id != 0 || data_shm_offset != 0) {
    data = GetSharedMemoryAs<const void*>(data_shm_id, data_shm_offset, size);
    if (!data) {
      return error::kOutOfBounds;
    }
  }
  buffer_manager()->ValidateAndDoBufferData(&state_, target, size, data, usage);
  return error::kNoError;
}
