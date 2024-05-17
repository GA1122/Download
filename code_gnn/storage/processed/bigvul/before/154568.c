error::Error GLES2DecoderPassthroughImpl::DoBindSampler(GLuint unit,
                                                        GLuint sampler) {
  api()->glBindSamplerFn(unit, GetSamplerServiceID(sampler, resources_));
  return error::kNoError;
}
