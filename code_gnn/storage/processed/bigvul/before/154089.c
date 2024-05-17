void GLES2DecoderImpl::DoUniform4uiv(GLint fake_location,
                                     GLsizei count,
                                     const volatile GLuint* value) {
  GLenum type = 0;
  GLint real_location = -1;
  if (!PrepForSetUniformByLocation(fake_location,
                                   "glUniform4uiv",
                                   Program::kUniform4ui,
                                   &real_location,
                                   &type,
                                   &count)) {
    return;
  }
  api()->glUniform4uivFn(real_location, count,
                         const_cast<const GLuint*>(value));
}
