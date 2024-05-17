error::Error GLES2DecoderPassthroughImpl::DoDisable(GLenum cap) {
  api()->glDisableFn(cap);
  return error::kNoError;
}
