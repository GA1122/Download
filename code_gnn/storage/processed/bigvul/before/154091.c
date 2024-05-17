void GLES2DecoderImpl::DoUniformMatrix2x3fv(GLint fake_location,
                                            GLsizei count,
                                            GLboolean transpose,
                                            const volatile GLfloat* value) {
  GLenum type = 0;
  GLint real_location = -1;
  if (!PrepForSetUniformByLocation(fake_location,
                                   "glUniformMatrix2x3fv",
                                   Program::kUniformMatrix2x3f,
                                   &real_location,
                                   &type,
                                   &count)) {
    return;
  }
  api()->glUniformMatrix2x3fvFn(real_location, count, transpose,
                                const_cast<const GLfloat*>(value));
}
