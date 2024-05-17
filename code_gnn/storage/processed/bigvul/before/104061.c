void GLES2DecoderImpl::DoUniformMatrix3fv(
  GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) {
  GLenum type = 0;
  if (!PrepForSetUniformByLocation(
      location, "glUniformMatrix3fv", &type, &count)) {
    return;
  }
  glUniformMatrix3fv (location, count, transpose, value);
}
