error::Error GLES2DecoderPassthroughImpl::DoFinish() {
  api()->glFinishFn();

  error::Error error = ProcessReadPixels(true);
  if (error != error::kNoError) {
    return error;
  }
  return ProcessQueries(true);
}
