error::Error GLES2DecoderPassthroughImpl::DoSamplerParameterf(GLuint sampler,
                                                              GLenum pname,
                                                              GLfloat param) {
  api()->glSamplerParameterfFn(GetSamplerServiceID(sampler, resources_), pname,
                               param);
  return error::kNoError;
}
