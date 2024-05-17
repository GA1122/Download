error::Error GLES2DecoderPassthroughImpl::DoLineWidth(GLfloat width) {
  api()->glLineWidthFn(width);
  return error::kNoError;
}
