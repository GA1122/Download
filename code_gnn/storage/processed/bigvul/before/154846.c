error::Error GLES2DecoderPassthroughImpl::DoUniform3uiv(
    GLint location,
    GLsizei count,
    const volatile GLuint* v) {
  api()->glUniform3uivFn(location, count, const_cast<const GLuint*>(v));
  return error::kNoError;
}
