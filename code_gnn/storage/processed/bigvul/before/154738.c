error::Error GLES2DecoderPassthroughImpl::DoIsRenderbuffer(GLuint renderbuffer,
                                                           uint32_t* result) {
  *result = api()->glIsRenderbufferEXTFn(
      GetRenderbufferServiceID(api(), renderbuffer, resources_, false));
  return error::kNoError;
}
