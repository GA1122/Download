error::Error GLES2DecoderPassthroughImpl::DoTexParameteriv(
    GLenum target,
    GLenum pname,
    const volatile GLint* params) {
  std::array<GLint, 1> params_copy{{params[0]}};
  api()->glTexParameterivRobustANGLEFn(target, pname,
                                       static_cast<GLsizei>(params_copy.size()),
                                       params_copy.data());
  return error::kNoError;
}
