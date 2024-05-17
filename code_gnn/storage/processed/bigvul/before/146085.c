void WebGL2RenderingContextBase::deleteTransformFeedback(
    WebGLTransformFeedback* feedback) {
  if (feedback == transform_feedback_binding_)
    transform_feedback_binding_ = default_transform_feedback_;

  DeleteObject(feedback);
}
