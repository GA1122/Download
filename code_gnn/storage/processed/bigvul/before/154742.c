error::Error GLES2DecoderPassthroughImpl::DoIsTexture(GLuint texture,
                                                      uint32_t* result) {
  *result = api()->glIsTextureFn(
      GetTextureServiceID(api(), texture, resources_, false));
  return error::kNoError;
}
