error::Error GLES2DecoderPassthroughImpl::DoBeginTransformFeedback(
    GLenum primitivemode) {
  api()->glBeginTransformFeedbackFn(primitivemode);
  return error::kNoError;
}
