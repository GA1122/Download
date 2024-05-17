void GLES2DecoderImpl::DoUniform2iv(GLint fake_location,
                                    GLsizei count,
                                    const volatile GLint* value) {
  GLenum type = 0;
  GLint real_location = -1;
  if (!PrepForSetUniformByLocation(fake_location,
                                   "glUniform2iv",
                                   Program::kUniform2i,
                                   &real_location,
                                   &type,
                                   &count)) {
    return;
  }
  api()->glUniform2ivFn(real_location, count, const_cast<const GLint*>(value));
}
