error::Error GLES2DecoderImpl::HandleMapBufferRange(
    uint32_t immediate_data_size, const void* cmd_data) {
  if (!unsafe_es3_apis_enabled()) {
    return error::kUnknownCommand;
  }
  const gles2::cmds::MapBufferRange& c =
      *static_cast<const gles2::cmds::MapBufferRange*>(cmd_data);
  GLenum target = static_cast<GLenum>(c.target);
  GLbitfield access = static_cast<GLbitfield>(c.access);
  GLintptr offset = static_cast<GLintptr>(c.offset);
  GLsizeiptr size = static_cast<GLsizeiptr>(c.size);

  typedef cmds::MapBufferRange::Result Result;
  Result* result = GetSharedMemoryAs<Result*>(
      c.result_shm_id, c.result_shm_offset, sizeof(*result));
  if (!result) {
    return error::kOutOfBounds;
  }
  if (*result != 0) {
    *result = 0;
    return error::kInvalidArguments;
  }
  int8_t* mem =
      GetSharedMemoryAs<int8_t*>(c.data_shm_id, c.data_shm_offset, size);
  if (!mem) {
    return error::kOutOfBounds;
  }

  GLbitfield mask = GL_MAP_INVALIDATE_BUFFER_BIT;
  if ((access & mask) == mask) {
    access = (access & ~GL_MAP_INVALIDATE_BUFFER_BIT);
    access = (access | GL_MAP_INVALIDATE_RANGE_BIT);
  }
  mask = GL_MAP_READ_BIT | GL_MAP_UNSYNCHRONIZED_BIT;
  if ((access & mask) == mask) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, "MapBufferRange",
                       "incompatible access bits");
    return error::kNoError;
  }
  access = (access & ~GL_MAP_UNSYNCHRONIZED_BIT);
  if ((access & GL_MAP_WRITE_BIT) == GL_MAP_WRITE_BIT &&
      (access & GL_MAP_INVALIDATE_RANGE_BIT) == 0) {
    access = (access | GL_MAP_READ_BIT);
  }
  void* ptr = glMapBufferRange(target, offset, size, access);
  if (ptr == nullptr) {
    return error::kNoError;
  }
  Buffer* buffer = buffer_manager()->GetBufferInfoForTarget(&state_, target);
  DCHECK(buffer);
  buffer->SetMappedRange(offset, size, access, ptr,
                         GetSharedMemoryBuffer(c.data_shm_id));
  if ((access & GL_MAP_INVALIDATE_RANGE_BIT) == 0) {
    memcpy(mem, ptr, size);
  }
  *result = 1;
  return error::kNoError;
}
