void GLES2DecoderImpl::DoUniform1uiv(GLint fake_location,
                                     GLsizei count,
                                     const volatile GLuint* value) {
  GLenum type = 0;
  GLint real_location = -1;
  if (!PrepForSetUniformByLocation(fake_location,
                                   "glUniform1uiv",
                                   Program::kUniform1ui,
                                   &real_location,
                                   &type,
                                   &count)) {
    return;
  }
  api()->glUniform1uivFn(real_location, count,
                         const_cast<const GLuint*>(value));
}
