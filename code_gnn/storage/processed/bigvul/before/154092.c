void GLES2DecoderImpl::DoUniformMatrix2x4fv(GLint fake_location,
                                            GLsizei count,
                                            GLboolean transpose,
                                            const volatile GLfloat* value) {
  GLenum type = 0;
  GLint real_location = -1;
  if (!PrepForSetUniformByLocation(fake_location,
                                   "glUniformMatrix2x4fv",
                                   Program::kUniformMatrix2x4f,
                                   &real_location,
                                   &type,
                                   &count)) {
    return;
  }
  api()->glUniformMatrix2x4fvFn(real_location, count, transpose,
                                const_cast<const GLfloat*>(value));
}
