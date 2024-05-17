error::Error GLES2DecoderPassthroughImpl::DoResumeTransformFeedback() {
  api()->glResumeTransformFeedbackFn();
  return error::kNoError;
}
