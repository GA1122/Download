error::Error GLES2DecoderPassthroughImpl::DoSamplerParameteri(GLuint sampler,
                                                              GLenum pname,
                                                              GLint param) {
  api()->glSamplerParameteriFn(GetSamplerServiceID(sampler, resources_), pname,
                               param);
  return error::kNoError;
}
