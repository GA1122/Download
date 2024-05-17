error::Error GLES2DecoderPassthroughImpl::DoUniform4iv(
    GLint location,
    GLsizei count,
    const volatile GLint* v) {
  api()->glUniform4ivFn(location, count, const_cast<const GLint*>(v));
  return error::kNoError;
}
