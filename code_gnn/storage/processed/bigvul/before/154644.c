error::Error GLES2DecoderPassthroughImpl::DoDrawElementsInstancedANGLE(
    GLenum mode,
    GLsizei count,
    GLenum type,
    const void* indices,
    GLsizei primcount) {
  BindPendingImagesForSamplersIfNeeded();
  api()->glDrawElementsInstancedANGLEFn(mode, count, type, indices, primcount);
  return error::kNoError;
}
