void GLES2DecoderImpl::DoUniform2fv(
    GLint location, GLsizei count, const GLfloat* value) {
  GLenum type = 0;
  if (!PrepForSetUniformByLocation(location, "glUniform2fv", &type, &count)) {
    return;
  }
  if (type == GL_BOOL_VEC2) {
    GLsizei num_values = count * 2;
    scoped_array<GLint> temp(new GLint[num_values]);
    for (GLsizei ii = 0; ii < num_values; ++ii) {
      temp[ii] = static_cast<GLint>(value[ii] != 0.0f);
    }
    glUniform2iv(location, count, temp.get());
  } else {
    glUniform2fv(location, count, value);
  }
}
