error::Error GLES2DecoderPassthroughImpl::DoGetSamplerParameteriv(
    GLuint sampler,
    GLenum pname,
    GLsizei bufsize,
    GLsizei* length,
    GLint* params) {
  api()->glGetSamplerParameterivRobustANGLEFn(
      GetSamplerServiceID(sampler, resources_), pname, bufsize, length, params);
  return error::kNoError;
}
