error::Error GLES2DecoderPassthroughImpl::DoBlendEquation(GLenum mode) {
  api()->glBlendEquationFn(mode);
  return error::kNoError;
}
