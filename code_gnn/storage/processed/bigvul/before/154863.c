error::Error GLES2DecoderPassthroughImpl::DoUniformMatrix4x3fv(
    GLint location,
    GLsizei count,
    GLboolean transpose,
    const volatile GLfloat* value) {
  api()->glUniformMatrix4x3fvFn(location, count, transpose,
                                const_cast<const GLfloat*>(value));
  return error::kNoError;
}
