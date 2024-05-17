error::Error GLES2DecoderPassthroughImpl::DoDepthFunc(GLenum func) {
  api()->glDepthFuncFn(func);
  return error::kNoError;
}
