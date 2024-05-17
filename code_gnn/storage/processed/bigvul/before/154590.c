error::Error GLES2DecoderPassthroughImpl::DoClearDepthf(GLclampf depth) {
  api()->glClearDepthfFn(depth);
  return error::kNoError;
}
