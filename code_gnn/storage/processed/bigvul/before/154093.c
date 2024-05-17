void GLES2DecoderImpl::DoUniformMatrix3fv(GLint fake_location,
                                          GLsizei count,
                                          GLboolean transpose,
                                          const volatile GLfloat* value) {
  GLenum type = 0;
  GLint real_location = -1;
  if (transpose && !feature_info_->IsWebGL2OrES3Context()) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, "glUniformMatrix3fv", "transpose not FALSE");
    return;
  }
  if (!PrepForSetUniformByLocation(fake_location,
                                   "glUniformMatrix3fv",
                                   Program::kUniformMatrix3f,
                                   &real_location,
                                   &type,
                                   &count)) {
    return;
  }
  api()->glUniformMatrix3fvFn(real_location, count, transpose,
                              const_cast<const GLfloat*>(value));
}
