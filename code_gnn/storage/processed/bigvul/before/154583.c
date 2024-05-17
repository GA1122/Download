error::Error GLES2DecoderPassthroughImpl::DoBufferSubData(GLenum target,
                                                          GLintptr offset,
                                                          GLsizeiptr size,
                                                          const void* data) {
  api()->glBufferSubDataFn(target, offset, size, data);
  return error::kNoError;
}
