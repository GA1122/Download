void GLES2DecoderImpl::DoUniform3uiv(GLint fake_location,
                                     GLsizei count,
                                     const volatile GLuint* value) {
  GLenum type = 0;
  GLint real_location = -1;
  if (!PrepForSetUniformByLocation(fake_location,
                                   "glUniform3uiv",
                                   Program::kUniform3ui,
                                   &real_location,
                                   &type,
                                   &count)) {
    return;
  }
  api()->glUniform3uivFn(real_location, count,
                         const_cast<const GLuint*>(value));
}
