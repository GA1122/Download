error::Error GLES2DecoderPassthroughImpl::DoClear(GLbitfield mask) {
  api()->glClearFn(mask);
  return error::kNoError;
}
