void GLES2DecoderImpl::DoEndTransformFeedback() {
  const char* function_name = "glEndTransformFeedback";
  DCHECK(state_.bound_transform_feedback.get());
  if (!state_.bound_transform_feedback->active()) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, function_name,
                       "transform feedback is not active");
    return;
  }
  state_.bound_transform_feedback->DoEndTransformFeedback();
}
