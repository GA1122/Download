error::Error GLES2DecoderPassthroughImpl::DoUniform1iv(
    GLint location,
    GLsizei count,
    const volatile GLint* v) {
  api()->glUniform1ivFn(location, count, const_cast<const GLint*>(v));
  return error::kNoError;
}
