error::Error GLES2DecoderPassthroughImpl::DoSamplerParameteriv(
    GLuint sampler,
    GLenum pname,
    const volatile GLint* params) {
  std::array<GLint, 1> params_copy{{params[0]}};
  api()->glSamplerParameterivRobustANGLEFn(
      GetSamplerServiceID(sampler, resources_), pname,
      static_cast<GLsizei>(params_copy.size()), params_copy.data());
  return error::kNoError;
}
