void GLES2DecoderImpl::DoUniform3iv(
    GLint location, GLsizei count, const GLint* value) {
  GLenum type = 0;
  if (!PrepForSetUniformByLocation(location, "glUniform3iv", &type, &count)) {
    return;
  }
  glUniform3iv(location, count, value);
}
