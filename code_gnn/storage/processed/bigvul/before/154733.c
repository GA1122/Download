error::Error GLES2DecoderPassthroughImpl::DoIsBuffer(GLuint buffer,
                                                     uint32_t* result) {
  *result =
      api()->glIsBufferFn(GetBufferServiceID(api(), buffer, resources_, false));
  return error::kNoError;
}
