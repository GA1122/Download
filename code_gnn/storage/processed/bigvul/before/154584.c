error::Error GLES2DecoderPassthroughImpl::DoCheckFramebufferStatus(
    GLenum target,
    uint32_t* result) {
  *result = api()->glCheckFramebufferStatusEXTFn(target);
  return error::kNoError;
}
