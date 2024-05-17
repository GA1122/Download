error::Error GLES2DecoderPassthroughImpl::DoCullFace(GLenum mode) {
  api()->glCullFaceFn(mode);
  return error::kNoError;
}
