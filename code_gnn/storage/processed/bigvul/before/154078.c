void GLES2DecoderImpl::DoUniform1fv(GLint fake_location,
                                    GLsizei count,
                                    const volatile GLfloat* value) {
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
    std::unique_ptr<GLint[]> temp(new GLint[count]);
    for (GLsizei ii = 0; ii < count; ++ii) {
      temp[ii] = static_cast<GLint>(value[ii] != 0.0f);
    }
    api()->glUniform1ivFn(real_location, count, temp.get());
  } else {
    api()->glUniform1fvFn(real_location, count,
                          const_cast<const GLfloat*>(value));
  }
}
