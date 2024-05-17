error::Error GLES2DecoderPassthroughImpl::DoGenTransformFeedbacks(
    GLsizei n,
    volatile GLuint* ids) {
  return GenHelper(n, ids, &transform_feedback_id_map_,
                   [this](GLsizei n, GLuint* transform_feedbacks) {
                     api()->glGenTransformFeedbacksFn(n, transform_feedbacks);
                   });
}
