void GLES2DecoderImpl::DoUniform2fv(GLint fake_location,
                                    GLsizei count,
                                    const volatile GLfloat* value) {
  GLenum type = 0;
  GLint real_location = -1;
  if (!PrepForSetUniformByLocation(fake_location,
                                   "glUniform2fv",
                                   Program::kUniform2f,
                                   &real_location,
                                   &type,
                                   &count)) {
    return;
  }
  if (type == GL_BOOL_VEC2) {
    GLsizei num_values = count * 2;
    std::unique_ptr<GLint[]> temp(new GLint[num_values]);
    for (GLsizei ii = 0; ii < num_values; ++ii) {
      temp[ii] = static_cast<GLint>(value[ii] != 0.0f);
    }
    api()->glUniform2ivFn(real_location, count, temp.get());
  } else {
    api()->glUniform2fvFn(real_location, count,
                          const_cast<const GLfloat*>(value));
  }
}
