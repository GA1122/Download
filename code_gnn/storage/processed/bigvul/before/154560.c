error::Error GLES2DecoderPassthroughImpl::DoBindBufferBase(GLenum target,
                                                           GLuint index,
                                                           GLuint buffer) {
  CheckErrorCallbackState();
  api()->glBindBufferBaseFn(
      target, index,
      GetBufferServiceID(api(), buffer, resources_, bind_generates_resource_));
  if (CheckErrorCallbackState()) {
    return error::kNoError;
  }

  DCHECK(bound_buffers_.find(target) != bound_buffers_.end());
  bound_buffers_[target] = buffer;

  return error::kNoError;
}
