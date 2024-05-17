error::Error GLES2DecoderPassthroughImpl::DoDrawElements(GLenum mode,
                                                         GLsizei count,
                                                         GLenum type,
                                                         const void* indices) {
  BindPendingImagesForSamplersIfNeeded();
  api()->glDrawElementsFn(mode, count, type, indices);
  return error::kNoError;
}
