error::Error GLES2DecoderPassthroughImpl::DoFlushMappedBufferRange(
    GLenum target,
    GLintptr offset,
    GLsizeiptr size) {
  auto bound_buffers_iter = bound_buffers_.find(target);
  if (bound_buffers_iter == bound_buffers_.end() ||
      bound_buffers_iter->second == 0) {
    InsertError(GL_INVALID_OPERATION, "No buffer bound to this target.");
    return error::kNoError;
  }

  GLuint client_buffer = bound_buffers_iter->second;
  auto mapped_buffer_info_iter =
      resources_->mapped_buffer_map.find(client_buffer);
  if (mapped_buffer_info_iter == resources_->mapped_buffer_map.end()) {
    InsertError(GL_INVALID_OPERATION, "Buffer is not mapped.");
    return error::kNoError;
  }

  const MappedBuffer& map_info = mapped_buffer_info_iter->second;

  if (offset < 0) {
    InsertError(GL_INVALID_VALUE, "Offset cannot be negative.");
    return error::kNoError;
  }

  if (size < 0) {
    InsertError(GL_INVALID_VALUE, "Size cannot be negative.");
    return error::kNoError;
  }

  base::CheckedNumeric<size_t> range_start(offset);
  base::CheckedNumeric<size_t> range_end = offset + size;
  if (!range_end.IsValid() || range_end.ValueOrDefault(0) > map_info.size) {
    InsertError(GL_INVALID_OPERATION,
                "Flush range is not within the original mapping size.");
    return error::kNoError;
  }

  uint8_t* mem = GetSharedMemoryAs<uint8_t*>(
      map_info.data_shm_id, map_info.data_shm_offset, map_info.size);
  if (!mem) {
    return error::kOutOfBounds;
  }

  memcpy(map_info.map_ptr + offset, mem + offset, size);
  api()->glFlushMappedBufferRangeFn(target, offset, size);

  return error::kNoError;
}
