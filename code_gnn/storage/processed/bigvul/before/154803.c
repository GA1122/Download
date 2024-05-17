error::Error GLES2DecoderPassthroughImpl::DoStencilMask(GLuint mask) {
  api()->glStencilMaskFn(mask);
  return error::kNoError;
}
