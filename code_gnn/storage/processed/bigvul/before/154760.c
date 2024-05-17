error::Error GLES2DecoderPassthroughImpl::DoPauseTransformFeedback() {
  api()->glPauseTransformFeedbackFn();
  return error::kNoError;
}
