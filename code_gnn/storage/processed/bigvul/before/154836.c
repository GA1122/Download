error::Error GLES2DecoderPassthroughImpl::DoUniform2fv(
    GLint location,
    GLsizei count,
    const volatile GLfloat* v) {
  api()->glUniform2fvFn(location, count, const_cast<const GLfloat*>(v));
  return error::kNoError;
}
