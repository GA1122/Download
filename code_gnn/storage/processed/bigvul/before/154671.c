error::Error GLES2DecoderPassthroughImpl::DoGenerateMipmap(GLenum target) {
  api()->glGenerateMipmapEXTFn(target);
  return error::kNoError;
}
