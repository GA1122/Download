error::Error GLES2DecoderPassthroughImpl::DoUniformMatrix2fv(
    GLint location,
    GLsizei count,
    GLboolean transpose,
    const volatile GLfloat* value) {
  api()->glUniformMatrix2fvFn(location, count, transpose,
                              const_cast<const GLfloat*>(value));
  return error::kNoError;
}
