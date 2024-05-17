error::Error GLES2DecoderPassthroughImpl::DoMapBufferRange(
    GLenum target,
    GLintptr offset,
    GLsizeiptr size,
    GLbitfield access,
    void* ptr,
    int32_t data_shm_id,
    uint32_t data_shm_offset,
    uint32_t* result) {
  CheckErrorCallbackState();

  GLbitfield filtered_access = access;

  filtered_access = (filtered_access & ~GL_MAP_UNSYNCHRONIZED_BIT);

  if ((filtered_access & GL_MAP_INVALIDATE_BUFFER_BIT) != 0) {
    filtered_access = (filtered_access & ~GL_MAP_INVALIDATE_BUFFER_BIT);
    filtered_access = (filtered_access | GL_MAP_INVALIDATE_RANGE_BIT);
  }
  if ((filtered_access & GL_MAP_INVALIDATE_RANGE_BIT) == 0) {
    filtered_access = (filtered_access | GL_MAP_READ_BIT);
  }

  void* mapped_ptr =
      api()->glMapBufferRangeFn(target, offset, size, filtered_access);
  if (CheckErrorCallbackState() || mapped_ptr == nullptr) {
    *result = 0;
    return error::kNoError;
  }

  if ((filtered_access & GL_MAP_INVALIDATE_RANGE_BIT) == 0) {
    memcpy(ptr, mapped_ptr, size);
  }

  DCHECK(bound_buffers_.find(target) != bound_buffers_.end());
  GLuint client_buffer = bound_buffers_.at(target);

  MappedBuffer mapped_buffer_info;
  mapped_buffer_info.size = size;
  mapped_buffer_info.original_access = access;
  mapped_buffer_info.filtered_access = filtered_access;
  mapped_buffer_info.map_ptr = static_cast<uint8_t*>(mapped_ptr);
  mapped_buffer_info.data_shm_id = data_shm_id;
  mapped_buffer_info.data_shm_offset = data_shm_offset;

  DCHECK(resources_->mapped_buffer_map.find(client_buffer) ==
         resources_->mapped_buffer_map.end());
  resources_->mapped_buffer_map.insert(
      std::make_pair(client_buffer, mapped_buffer_info));

  *result = 1;
  return error::kNoError;
}
