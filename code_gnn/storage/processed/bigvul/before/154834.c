error::Error GLES2DecoderPassthroughImpl::DoUniform1uiv(
    GLint location,
    GLsizei count,
    const volatile GLuint* v) {
  api()->glUniform1uivFn(location, count, const_cast<const GLuint*>(v));
  return error::kNoError;
}
