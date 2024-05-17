void WebGL2RenderingContextBase::endTransformFeedback() {
  if (isContextLost())
    return;

  ContextGL()->EndTransformFeedback();

  if (current_program_)
    current_program_->DecreaseActiveTransformFeedbackCount();
}
