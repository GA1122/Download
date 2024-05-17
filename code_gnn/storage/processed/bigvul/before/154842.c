error::Error GLES2DecoderPassthroughImpl::DoUniform3fv(
    GLint location,
    GLsizei count,
    const volatile GLfloat* v) {
  api()->glUniform3fvFn(location, count, const_cast<const GLfloat*>(v));
  return error::kNoError;
}
