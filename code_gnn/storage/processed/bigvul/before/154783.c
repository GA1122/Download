error::Error GLES2DecoderPassthroughImpl::DoSamplerParameterfv(
    GLuint sampler,
    GLenum pname,
    const volatile GLfloat* params) {
  std::array<GLfloat, 1> params_copy{{params[0]}};
  api()->glSamplerParameterfvRobustANGLEFn(
      GetSamplerServiceID(sampler, resources_), pname,
      static_cast<GLsizei>(params_copy.size()), params_copy.data());
  return error::kNoError;
}
