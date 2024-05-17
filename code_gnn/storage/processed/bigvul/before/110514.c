void GLES2DecoderImpl::DoUniform2iv(
    GLint fake_location, GLsizei count, const GLint* value) {
  GLenum type = 0;
  GLint real_location = -1;
  if (!PrepForSetUniformByLocation(
      fake_location, "glUniform2iv", &real_location, &type, &count)) {
    return;
  }
  glUniform2iv(real_location, count, value);
}
