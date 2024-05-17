void GLES2DecoderImpl::DoUniformMatrix4fv(GLint fake_location,
                                          GLsizei count,
                                          GLboolean transpose,
                                          const volatile GLfloat* value) {
  GLenum type = 0;
  GLint real_location = -1;
  if (transpose && !feature_info_->IsWebGL2OrES3Context()) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, "glUniformMatrix4fv", "transpose not FALSE");
    return;
  }
  if (!PrepForSetUniformByLocation(fake_location,
                                   "glUniformMatrix4fv",
                                   Program::kUniformMatrix4f,
                                   &real_location,
                                   &type,
                                   &count)) {
    return;
  }
  api()->glUniformMatrix4fvFn(real_location, count, transpose,
                              const_cast<const GLfloat*>(value));
}
