void WebGL2RenderingContextBase::bindTransformFeedback(
    GLenum target,
    WebGLTransformFeedback* feedback) {
  bool deleted;
  if (!CheckObjectToBeBound("bindTransformFeedback", feedback, deleted))
    return;
  if (deleted) {
    SynthesizeGLError(GL_INVALID_OPERATION, "bindTransformFeedback",
                      "attempted to bind a deleted transform feedback object");
    return;
  }

  if (target != GL_TRANSFORM_FEEDBACK) {
    SynthesizeGLError(GL_INVALID_ENUM, "bindTransformFeedback",
                      "target must be TRANSFORM_FEEDBACK");
    return;
  }

  if (transform_feedback_binding_->active() &&
      !transform_feedback_binding_->paused()) {
    SynthesizeGLError(GL_INVALID_OPERATION, "bindTransformFeedback",
                      "transform feedback is active and not paused");
    return;
  }

  WebGLTransformFeedback* feedback_to_be_bound;
  if (feedback) {
    feedback_to_be_bound = feedback;
    feedback_to_be_bound->SetTarget(target);
  } else {
    feedback_to_be_bound = default_transform_feedback_.Get();
  }

  transform_feedback_binding_ = feedback_to_be_bound;
  ContextGL()->BindTransformFeedback(target,
                                     ObjectOrZero(feedback_to_be_bound));
}
