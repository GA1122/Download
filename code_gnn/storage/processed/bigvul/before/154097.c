void GLES2DecoderImpl::DoUniformMatrix4x2fv(GLint fake_location,
                                            GLsizei count,
                                            GLboolean transpose,
                                            const volatile GLfloat* value) {
  GLenum type = 0;
  GLint real_location = -1;
  if (!PrepForSetUniformByLocation(fake_location,
                                   "glUniformMatrix4x2fv",
                                   Program::kUniformMatrix4x2f,
                                   &real_location,
                                   &type,
                                   &count)) {
    return;
  }
  api()->glUniformMatrix4x2fvFn(real_location, count, transpose,
                                const_cast<const GLfloat*>(value));
}
