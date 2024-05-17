error::Error GLES2DecoderPassthroughImpl::DoUnmapBuffer(GLenum target) {
  auto bound_buffers_iter = bound_buffers_.find(target);
  if (bound_buffers_iter == bound_buffers_.end()) {
    InsertError(GL_INVALID_ENUM, "Invalid buffer target.");
    return error::kNoError;
  }

  if (bound_buffers_iter->second == 0) {
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
  if ((map_info.filtered_access & GL_MAP_WRITE_BIT) != 0 &&
      (map_info.filtered_access & GL_MAP_FLUSH_EXPLICIT_BIT) == 0) {
    uint8_t* mem = GetSharedMemoryAs<uint8_t*>(
        map_info.data_shm_id, map_info.data_shm_offset, map_info.size);
    if (!mem) {
      return error::kOutOfBounds;
    }

    memcpy(map_info.map_ptr, mem, map_info.size);
  }

  api()->glUnmapBufferFn(target);

  resources_->mapped_buffer_map.erase(mapped_buffer_info_iter);

  return error::kNoError;
}
