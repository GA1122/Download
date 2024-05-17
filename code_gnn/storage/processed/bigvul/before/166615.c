void WebGL2RenderingContextBase::resumeTransformFeedback() {
  if (isContextLost())
    return;

  if (!transform_feedback_binding_->ValidateProgramForResume(
          current_program_)) {
    SynthesizeGLError(GL_INVALID_OPERATION, "resumeTransformFeedback",
                      "the current program is not the same as when "
                      "beginTransformFeedback was called");
    return;
  }
  if (!transform_feedback_binding_->active() ||
      !transform_feedback_binding_->paused()) {
    SynthesizeGLError(GL_INVALID_OPERATION, "resumeTransformFeedback",
                      "transform feedback is not active or not paused");
    return;
  }

  transform_feedback_binding_->SetPaused(false);
  ContextGL()->ResumeTransformFeedback();
}
