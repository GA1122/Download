error::Error GLES2DecoderPassthroughImpl::DoIsFramebuffer(GLuint framebuffer,
                                                          uint32_t* result) {
  *result = api()->glIsFramebufferEXTFn(
      GetFramebufferServiceID(api(), framebuffer, &framebuffer_id_map_, false));
  return error::kNoError;
}
