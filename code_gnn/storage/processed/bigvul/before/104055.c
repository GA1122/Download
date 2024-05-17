void GLES2DecoderImpl::DoUniform2iv(
    GLint location, GLsizei count, const GLint* value) {
  GLenum type = 0;
  if (!PrepForSetUniformByLocation(location, "glUniform2iv", &type, &count)) {
    return;
  }
  glUniform2iv(location, count, value);
}
