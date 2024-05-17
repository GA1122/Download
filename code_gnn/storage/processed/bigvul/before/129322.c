void GLES2DecoderImpl::DoUniform1fv(
    GLint fake_location, GLsizei count, const GLfloat* value) {
  GLenum type = 0;
  GLint real_location = -1;
  if (!PrepForSetUniformByLocation(fake_location,
                                   "glUniform1fv",
                                   Program::kUniform1f,
                                   &real_location,
                                   &type,
                                   &count)) {
    return;
  }
  if (type == GL_BOOL) {
    scoped_ptr<GLint[]> temp(new GLint[count]);
    for (GLsizei ii = 0; ii < count; ++ii) {
      temp[ii] = static_cast<GLint>(value[ii] != 0.0f);
    }
    DoUniform1iv(real_location, count, temp.get());
  } else {
    glUniform1fv(real_location, count, value);
  }
}
