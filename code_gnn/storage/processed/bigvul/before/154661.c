error::Error GLES2DecoderPassthroughImpl::DoFrontFace(GLenum mode) {
  api()->glFrontFaceFn(mode);
  return error::kNoError;
}
