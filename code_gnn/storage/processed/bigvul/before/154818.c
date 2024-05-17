error::Error GLES2DecoderPassthroughImpl::DoTexParameterfv(
    GLenum target,
    GLenum pname,
    const volatile GLfloat* params) {
  std::array<GLfloat, 1> params_copy{{params[0]}};
  api()->glTexParameterfvRobustANGLEFn(target, pname,
                                       static_cast<GLsizei>(params_copy.size()),
                                       params_copy.data());
  return error::kNoError;
}
