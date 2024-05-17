void GLES2DecoderImpl::DoUniform1fv(
    GLint location, GLsizei count, const GLfloat* value) {
  GLenum type = 0;
  if (!PrepForSetUniformByLocation(location, "glUniform1fv", &type, &count)) {
    return;
  }
  if (type == GL_BOOL) {
    scoped_array<GLint> temp(new GLint[count]);
    for (GLsizei ii = 0; ii < count; ++ii) {
      temp[ii] = static_cast<GLint>(value[ii] != 0.0f);
    }
    DoUniform1iv(location, count, temp.get());
  } else {
    glUniform1fv(location, count, value);
  }
}
