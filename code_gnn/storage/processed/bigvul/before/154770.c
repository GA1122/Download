error::Error GLES2DecoderPassthroughImpl::DoReadPixels(GLint x,
                                                       GLint y,
                                                       GLsizei width,
                                                       GLsizei height,
                                                       GLenum format,
                                                       GLenum type,
                                                       GLsizei bufsize,
                                                       GLsizei* length,
                                                       GLsizei* columns,
                                                       GLsizei* rows,
                                                       void* pixels,
                                                       int32_t* success) {
  CheckErrorCallbackState();
  ScopedPackStateRowLengthReset reset_row_length(
      api(), bufsize != 0 && feature_info_->gl_version_info().is_es3);
  api()->glReadPixelsRobustANGLEFn(x, y, width, height, format, type, bufsize,
                                   length, columns, rows, pixels);
  *success = CheckErrorCallbackState() ? 0 : 1;
  return error::kNoError;
}
