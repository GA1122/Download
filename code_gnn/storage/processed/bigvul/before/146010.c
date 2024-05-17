bool WebGL2RenderingContextBase::IsBufferBoundToTransformFeedback(
    WebGLBuffer* buffer) {
  DCHECK(buffer);
  return transform_feedback_binding_->IsBufferBoundToTransformFeedback(buffer);
}
