void GLES2DecoderImpl::DoUniform2uiv(GLint fake_location,
                                     GLsizei count,
                                     const volatile GLuint* value) {
  GLenum type = 0;
  GLint real_location = -1;
  if (!PrepForSetUniformByLocation(fake_location,
                                   "glUniform2uiv",
                                   Program::kUniform2ui,
                                   &real_location,
                                   &type,
                                   &count)) {
    return;
  }
  api()->glUniform2uivFn(real_location, count,
                         const_cast<const GLuint*>(value));
}
