void GLES2DecoderImpl::DoUniformMatrix2fv(
  GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) {
  GLenum type = 0;
  if (!PrepForSetUniformByLocation(
      location, "glUniformMatrix2fv", &type, &count)) {
    return;
  }
  glUniformMatrix2fv (location, count, transpose, value);
}
