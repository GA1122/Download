void WebGL2RenderingContextBase::beginTransformFeedback(GLenum primitive_mode) {
  if (isContextLost())
    return;
  if (!ValidateTransformFeedbackPrimitiveMode("beginTransformFeedback",
                                              primitive_mode))
    return;
  if (!current_program_) {
    SynthesizeGLError(GL_INVALID_OPERATION, "beginTransformFeedback",
                      "no program object is active");
    return;
  }
  if (transform_feedback_binding_->active()) {
    SynthesizeGLError(GL_INVALID_OPERATION, "beginTransformFeedback",
                      "transform feedback is already active");
    return;
  }
  int required_buffer_count =
      current_program_->GetRequiredTransformFeedbackBufferCount(this);
  if (required_buffer_count == 0) {
    SynthesizeGLError(GL_INVALID_OPERATION, "beginTransformFeedback",
                      "current active program does not specify any transform "
                      "feedback varyings to record");
    return;
  }
  if (!transform_feedback_binding_->HasEnoughBuffers(required_buffer_count)) {
    SynthesizeGLError(GL_INVALID_OPERATION, "beginTransformFeedback",
                      "not enough transform feedback buffers bound");
    return;
  }

  ContextGL()->BeginTransformFeedback(primitive_mode);
  current_program_->IncreaseActiveTransformFeedbackCount();
  transform_feedback_binding_->SetProgram(current_program_);
  transform_feedback_binding_->SetActive(true);
  transform_feedback_binding_->SetPaused(false);
}
