error::Error GLES2DecoderPassthroughImpl::DoDrawArraysInstancedANGLE(
    GLenum mode,
    GLint first,
    GLsizei count,
    GLsizei primcount) {
  BindPendingImagesForSamplersIfNeeded();
  api()->glDrawArraysInstancedANGLEFn(mode, first, count, primcount);
  return error::kNoError;
}
