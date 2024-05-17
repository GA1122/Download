error::Error GLES2DecoderPassthroughImpl::PatchGetBufferResults(GLenum target,
                                                                GLenum pname,
                                                                GLsizei bufsize,
                                                                GLsizei* length,
                                                                T* params) {
  if (pname != GL_BUFFER_ACCESS_FLAGS) {
    return error::kNoError;
  }

  DCHECK(bound_buffers_.find(target) != bound_buffers_.end());
  GLuint current_client_buffer = bound_buffers_[target];

  auto mapped_buffer_info_iter =
      resources_->mapped_buffer_map.find(current_client_buffer);
  if (mapped_buffer_info_iter == resources_->mapped_buffer_map.end()) {
    return error::kNoError;
  }

  DCHECK_GE(bufsize, 1);
  DCHECK_EQ(*length, 1);
  params[0] = mapped_buffer_info_iter->second.original_access;
  return error::kNoError;
}
