error::Error GLES2DecoderPassthroughImpl::DoBindBufferRange(GLenum target,
                                                            GLuint index,
                                                            GLuint buffer,
                                                            GLintptr offset,
                                                            GLsizeiptr size) {
  CheckErrorCallbackState();
  api()->glBindBufferRangeFn(
      target, index,
      GetBufferServiceID(api(), buffer, resources_, bind_generates_resource_),
      offset, size);
  if (CheckErrorCallbackState()) {
    return error::kNoError;
  }

  DCHECK(bound_buffers_.find(target) != bound_buffers_.end());
  bound_buffers_[target] = buffer;

  return error::kNoError;
}
