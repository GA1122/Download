void ContextState::RestoreProgramSettings(
    const ContextState* prev_state,
    bool restore_transform_feedback_bindings) const {
  bool flag = (restore_transform_feedback_bindings &&
               feature_info_->IsES3Capable());
  if (flag && prev_state) {
    if (prev_state->bound_transform_feedback.get() &&
        prev_state->bound_transform_feedback->active() &&
        !prev_state->bound_transform_feedback->paused()) {
      api()->glPauseTransformFeedbackFn();
    }
  }
  api()->glUseProgramFn(current_program.get() ? current_program->service_id()
                                              : 0);
  if (flag) {
    if (bound_transform_feedback.get()) {
      bound_transform_feedback->DoBindTransformFeedback(GL_TRANSFORM_FEEDBACK);
    } else {
      api()->glBindTransformFeedbackFn(GL_TRANSFORM_FEEDBACK, 0);
    }
  }
}
