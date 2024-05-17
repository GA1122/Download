error::Error GLES2DecoderPassthroughImpl::DoBufferData(GLenum target,
                                                       GLsizeiptr size,
                                                       const void* data,
                                                       GLenum usage) {
  CheckErrorCallbackState();
  api()->glBufferDataFn(target, size, data, usage);
  if (CheckErrorCallbackState()) {
    return error::kNoError;
  }

  resources_->mapped_buffer_map.erase(bound_buffers_[target]);

  return error::kNoError;
}
