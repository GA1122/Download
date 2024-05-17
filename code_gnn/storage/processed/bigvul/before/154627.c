error::Error GLES2DecoderPassthroughImpl::DoDeleteTransformFeedbacks(
    GLsizei n,
    const volatile GLuint* ids) {
  if (n < 0) {
    InsertError(GL_INVALID_VALUE, "n cannot be negative.");
    return error::kNoError;
  }
  return DeleteHelper(n, ids, &transform_feedback_id_map_,
                      [this](GLsizei n, GLuint* transform_feedbacks) {
                        api()->glDeleteTransformFeedbacksFn(
                            n, transform_feedbacks);
                      });
}
