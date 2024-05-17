void WebGL2RenderingContextBase::pauseTransformFeedback() {
  if (isContextLost())
    return;

  if (!transform_feedback_binding_->active()) {
    SynthesizeGLError(GL_INVALID_OPERATION, "pauseTransformFeedback",
                      "transform feedback is not active");
    return;
  }
  if (transform_feedback_binding_->paused()) {
    SynthesizeGLError(GL_INVALID_OPERATION, "pauseTransformFeedback",
                      "transform feedback is already paused");
    return;
  }

  transform_feedback_binding_->SetPaused(true);
  ContextGL()->PauseTransformFeedback();
}
