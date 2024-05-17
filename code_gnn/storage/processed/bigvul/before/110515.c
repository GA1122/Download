void GLES2DecoderImpl::DoUniform3fv(
    GLint fake_location, GLsizei count, const GLfloat* value) {
  GLenum type = 0;
  GLint real_location = -1;
  if (!PrepForSetUniformByLocation(
      fake_location, "glUniform3fv", &real_location, &type, &count)) {
    return;
  }
  if (type == GL_BOOL_VEC3) {
    GLsizei num_values = count * 3;
    scoped_array<GLint> temp(new GLint[num_values]);
    for (GLsizei ii = 0; ii < num_values; ++ii) {
      temp[ii] = static_cast<GLint>(value[ii] != 0.0f);
    }
    glUniform3iv(real_location, count, temp.get());
  } else {
    glUniform3fv(real_location, count, value);
  }
}
