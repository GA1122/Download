void GLES2DecoderImpl::DeleteTransformFeedbacksHelper(
    GLsizei n,
    const volatile GLuint* client_ids) {
  for (GLsizei ii = 0; ii < n; ++ii) {
    GLuint client_id = client_ids[ii];
    TransformFeedback* transform_feedback = GetTransformFeedback(client_id);
    if (transform_feedback) {
      if (transform_feedback->active()) {
        LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, "glDeleteTransformFeedbacks",
                           "Deleting transform feedback is active");
        return;
      }
      if (state_.bound_transform_feedback.get() == transform_feedback) {
        DCHECK(state_.default_transform_feedback.get());
        state_.default_transform_feedback->DoBindTransformFeedback(
            GL_TRANSFORM_FEEDBACK, state_.bound_transform_feedback.get(),
            state_.bound_transform_feedback_buffer.get());
        state_.bound_transform_feedback =
            state_.default_transform_feedback.get();
      }
      RemoveTransformFeedback(client_id);
    }
  }
}
