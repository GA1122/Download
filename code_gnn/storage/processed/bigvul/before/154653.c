error::Error GLES2DecoderPassthroughImpl::DoFlush() {
  api()->glFlushFn();

  error::Error error = ProcessReadPixels(false);
  if (error != error::kNoError) {
    return error;
  }
  return ProcessQueries(false);
}
