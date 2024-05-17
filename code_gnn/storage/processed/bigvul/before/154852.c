error::Error GLES2DecoderPassthroughImpl::DoUniform4uiv(
    GLint location,
    GLsizei count,
    const volatile GLuint* v) {
  api()->glUniform4uivFn(location, count, const_cast<const GLuint*>(v));
  return error::kNoError;
}
