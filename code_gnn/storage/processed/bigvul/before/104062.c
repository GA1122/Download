void GLES2DecoderImpl::DoUniformMatrix4fv(
  GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) {
  GLenum type = 0;
  if (!PrepForSetUniformByLocation(
      location, "glUniformMatrix4fv", &type, &count)) {
    return;
  }
  glUniformMatrix4fv (location, count, transpose, value);
}
