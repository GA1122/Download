void GLES2DecoderImpl::DoUniformMatrix3x2fv(GLint fake_location,
                                            GLsizei count,
                                            GLboolean transpose,
                                            const volatile GLfloat* value) {
  GLenum type = 0;
  GLint real_location = -1;
  if (!PrepForSetUniformByLocation(fake_location,
                                   "glUniformMatrix3x2fv",
                                   Program::kUniformMatrix3x2f,
                                   &real_location,
                                   &type,
                                   &count)) {
    return;
  }
  api()->glUniformMatrix3x2fvFn(real_location, count, transpose,
                                const_cast<const GLfloat*>(value));
}
