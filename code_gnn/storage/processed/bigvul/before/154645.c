error::Error GLES2DecoderPassthroughImpl::DoEnable(GLenum cap) {
  api()->glEnableFn(cap);
  return error::kNoError;
}
