error::Error GLES2DecoderPassthroughImpl::DoHint(GLenum target, GLenum mode) {
  api()->glHintFn(target, mode);
  return error::kNoError;
}
