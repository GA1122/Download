void GLES2DecoderImpl::DoUniformMatrix2fv(
    GLint fake_location, GLsizei count, GLboolean transpose,
    const GLfloat* value) {
  GLenum type = 0;
  GLint real_location = -1;
  if (!PrepForSetUniformByLocation(fake_location,
                                   "glUniformMatrix2fv",
                                   Program::kUniformMatrix2f,
                                   &real_location,
                                   &type,
                                   &count)) {
    return;
  }
  glUniformMatrix2fv(real_location, count, transpose, value);
}
