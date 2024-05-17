error::Error GLES2DecoderPassthroughImpl::DoReadBuffer(GLenum src) {
  api()->glReadBufferFn(src);
  return error::kNoError;
}
