void GLES2DecoderImpl::DoResumeTransformFeedback() {
  DCHECK(state_.bound_transform_feedback.get());
  if (!state_.bound_transform_feedback->active() ||
      !state_.bound_transform_feedback->paused()) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, "glResumeTransformFeedback",
                       "transform feedback is not active or not paused");
    return;
  }
  if (workarounds().rebind_transform_feedback_before_resume) {
    api()->glBindTransformFeedbackFn(GL_TRANSFORM_FEEDBACK, 0);
    api()->glBindTransformFeedbackFn(
        GL_TRANSFORM_FEEDBACK, state_.bound_transform_feedback->service_id());
  }
  state_.bound_transform_feedback->DoResumeTransformFeedback();
}
