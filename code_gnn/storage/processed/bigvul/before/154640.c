error::Error GLES2DecoderPassthroughImpl::DoDrawArrays(GLenum mode,
                                                       GLint first,
                                                       GLsizei count) {
  BindPendingImagesForSamplersIfNeeded();
  api()->glDrawArraysFn(mode, first, count);
  return error::kNoError;
}
