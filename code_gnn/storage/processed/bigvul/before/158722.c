error::Error GLES2DecoderImpl::HandleMapBufferRange(
    uint32_t immediate_data_size, const volatile void* cmd_data) {
  if (!feature_info_->IsWebGL2OrES3Context()) {
    return error::kUnknownCommand;
  }

  const char* func_name = "glMapBufferRange";
  const volatile gles2::cmds::MapBufferRange& c =
      *static_cast<const volatile gles2::cmds::MapBufferRange*>(cmd_data);
  GLenum target = static_cast<GLenum>(c.target);
  GLbitfield access = static_cast<GLbitfield>(c.access);
  GLintptr offset = static_cast<GLintptr>(c.offset);
  GLsizeiptr size = static_cast<GLsizeiptr>(c.size);
  uint32_t data_shm_id = static_cast<uint32_t>(c.data_shm_id);
  uint32_t data_shm_offset = static_cast<uint32_t>(c.data_shm_offset);

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
  if (!validators_->buffer_target.IsValid(target)) {
    LOCAL_SET_GL_ERROR_INVALID_ENUM(func_name, target, "target");
    return error::kNoError;
  }
  if (size == 0) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, func_name, "length is zero");
    return error::kNoError;
  }
  Buffer* buffer = buffer_manager()->RequestBufferAccess(
      &state_, target, offset, size, func_name);
  if (!buffer) {
    return error::kNoError;
  }
  if (state_.bound_transform_feedback->active() &&
      !state_.bound_transform_feedback->paused()) {
    size_t used_binding_count =
        state_.current_program->effective_transform_feedback_varyings().size();
    if (state_.bound_transform_feedback->UsesBuffer(
            used_binding_count, buffer)) {
      LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, func_name,
                         "active transform feedback is using this buffer");
      return error::kNoError;
    }
  }

  int8_t* mem =
      GetSharedMemoryAs<int8_t*>(data_shm_id, data_shm_offset, size);
  if (!mem) {
    return error::kOutOfBounds;
  }
  if (AnyOtherBitsSet(access, (GL_MAP_READ_BIT |
                               GL_MAP_WRITE_BIT |
                               GL_MAP_INVALIDATE_RANGE_BIT |
                               GL_MAP_INVALIDATE_BUFFER_BIT |
                               GL_MAP_FLUSH_EXPLICIT_BIT |
                               GL_MAP_UNSYNCHRONIZED_BIT))) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, func_name, "invalid access bits");
    return error::kNoError;
  }
  if (!AnyBitsSet(access, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, func_name,
        "neither MAP_READ_BIT nor MAP_WRITE_BIT is set");
    return error::kNoError;
  }
  if (AllBitsSet(access, GL_MAP_READ_BIT) &&
      AnyBitsSet(access, (GL_MAP_INVALIDATE_RANGE_BIT |
                          GL_MAP_INVALIDATE_BUFFER_BIT |
                          GL_MAP_UNSYNCHRONIZED_BIT))) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, func_name,
        "incompatible access bits with MAP_READ_BIT");
    return error::kNoError;
  }
  if (AllBitsSet(access, GL_MAP_FLUSH_EXPLICIT_BIT) &&
      !AllBitsSet(access, GL_MAP_WRITE_BIT)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, func_name,
        "MAP_FLUSH_EXPLICIT_BIT set without MAP_WRITE_BIT");
    return error::kNoError;
  }
  GLbitfield filtered_access = access;
  if (AllBitsSet(filtered_access, GL_MAP_INVALIDATE_BUFFER_BIT)) {
    filtered_access = (filtered_access & ~GL_MAP_INVALIDATE_BUFFER_BIT);
    filtered_access = (filtered_access | GL_MAP_INVALIDATE_RANGE_BIT);
  }
  filtered_access = (filtered_access & ~GL_MAP_UNSYNCHRONIZED_BIT);
  if (AllBitsSet(filtered_access, GL_MAP_WRITE_BIT) &&
      !AllBitsSet(filtered_access, GL_MAP_INVALIDATE_RANGE_BIT)) {
    filtered_access = (filtered_access | GL_MAP_READ_BIT);
  }
  void* ptr = api()->glMapBufferRangeFn(target, offset, size, filtered_access);
  if (ptr == nullptr) {
    LOCAL_COPY_REAL_GL_ERRORS_TO_WRAPPER(func_name);
    return error::kNoError;
  }
  buffer->SetMappedRange(offset, size, access, ptr,
                         GetSharedMemoryBuffer(data_shm_id),
                         static_cast<unsigned int>(data_shm_offset));
  if ((filtered_access & GL_MAP_INVALIDATE_RANGE_BIT) == 0) {
    memcpy(mem, ptr, size);
  }
  *result = 1;
  return error::kNoError;
}
