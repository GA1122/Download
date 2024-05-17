error::Error GLES2DecoderPassthroughImpl::DoIsSampler(GLuint sampler,
                                                      uint32_t* result) {
  *result = api()->glIsSamplerFn(GetSamplerServiceID(sampler, resources_));
  return error::kNoError;
}
