void GLES2DecoderImpl::DoUniformMatrix3fv(
    GLint fake_location, GLsizei count, GLboolean transpose,
    const GLfloat* value) {
  GLenum type = 0;
  GLint real_location = -1;
  if (!PrepForSetUniformByLocation(
      fake_location, "glUniformMatrix3fv", &real_location, &type, &count)) {
    return;
  }
  glUniformMatrix3fv(real_location, count, transpose, value);
}
