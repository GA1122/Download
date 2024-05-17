error::Error GLES2DecoderPassthroughImpl::DoEndTransformFeedback() {
  api()->glEndTransformFeedbackFn();
  return error::kNoError;
}
