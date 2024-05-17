error::Error GLES2DecoderPassthroughImpl::DoUniform1fv(
    GLint location,
    GLsizei count,
    const volatile GLfloat* v) {
  api()->glUniform1fvFn(location, count, const_cast<const GLfloat*>(v));
  return error::kNoError;
}
