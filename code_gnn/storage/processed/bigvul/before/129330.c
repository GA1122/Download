void GLES2DecoderImpl::DoUniform4iv(
    GLint fake_location, GLsizei count, const GLint* value) {
  GLenum type = 0;
  GLint real_location = -1;
  if (!PrepForSetUniformByLocation(fake_location,
                                   "glUniform4iv",
                                   Program::kUniform4i,
                                   &real_location,
                                   &type,
                                   &count)) {
    return;
  }
  glUniform4iv(real_location, count, value);
}
