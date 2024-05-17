error::Error GLES2DecoderPassthroughImpl::DoBindTransformFeedback(
    GLenum target,
    GLuint transformfeedback) {
  api()->glBindTransformFeedbackFn(
      target, GetTransformFeedbackServiceID(transformfeedback,
                                            &transform_feedback_id_map_));
  return error::kNoError;
}
