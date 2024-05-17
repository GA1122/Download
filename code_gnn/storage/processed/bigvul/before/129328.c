void GLES2DecoderImpl::DoUniform3iv(
    GLint fake_location, GLsizei count, const GLint* value) {
  GLenum type = 0;
  GLint real_location = -1;
  if (!PrepForSetUniformByLocation(fake_location,
                                   "glUniform3iv",
                                   Program::kUniform3i,
                                   &real_location,
                                   &type,
                                   &count)) {
    return;
  }
  glUniform3iv(real_location, count, value);
}
