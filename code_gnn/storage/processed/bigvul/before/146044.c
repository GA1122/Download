void WebGL2RenderingContextBase::beginTransformFeedback(GLenum primitive_mode) {
  if (isContextLost())
    return;
  if (!ValidateTransformFeedbackPrimitiveMode("beginTransformFeedback",
                                              primitive_mode))
    return;

  ContextGL()->BeginTransformFeedback(primitive_mode);

  if (current_program_)
    current_program_->IncreaseActiveTransformFeedbackCount();

  if (transform_feedback_binding_)
    transform_feedback_binding_->SetProgram(current_program_);
}
