void WebGL2RenderingContextBase::endTransformFeedback() {
  if (isContextLost())
    return;
  if (!transform_feedback_binding_->active()) {
    SynthesizeGLError(GL_INVALID_OPERATION, "endTransformFeedback",
                      "transform feedback is not active");
    return;
  }

  ContextGL()->EndTransformFeedback();

  transform_feedback_binding_->SetPaused(false);
  transform_feedback_binding_->SetActive(false);
  if (current_program_)
    current_program_->DecreaseActiveTransformFeedbackCount();
}
