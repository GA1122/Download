bool WebGL2RenderingContextBase::ValidateAndUpdateBufferBindBaseTarget(
    const char* function_name,
    GLenum target,
    GLuint index,
    WebGLBuffer* buffer) {
  if (!ValidateBufferBaseTarget(function_name, target))
    return false;

  if (buffer &&
      !ValidateBufferTargetCompatibility(function_name, target, buffer))
    return false;

  switch (target) {
    case GL_TRANSFORM_FEEDBACK_BUFFER:
      if (!transform_feedback_binding_->SetBoundIndexedTransformFeedbackBuffer(
              index, buffer)) {
        SynthesizeGLError(GL_INVALID_VALUE, function_name,
                          "index out of range");
        return false;
      }
      break;
    case GL_UNIFORM_BUFFER:
      if (index >= bound_indexed_uniform_buffers_.size()) {
        SynthesizeGLError(GL_INVALID_VALUE, function_name,
                          "index out of range");
        return false;
      }
      bound_indexed_uniform_buffers_[index] = buffer;
      bound_uniform_buffer_ = buffer;

      if (buffer) {
        if (index > max_bound_uniform_buffer_index_)
          max_bound_uniform_buffer_index_ = index;
      } else if (max_bound_uniform_buffer_index_ > 0 &&
                 index == max_bound_uniform_buffer_index_) {
        size_t i = max_bound_uniform_buffer_index_ - 1;
        for (; i > 0; --i) {
          if (bound_indexed_uniform_buffers_[i].Get())
            break;
        }
        max_bound_uniform_buffer_index_ = i;
      }
      break;
    default:
      NOTREACHED();
      break;
  }

  if (buffer && !buffer->GetInitialTarget())
    buffer->SetInitialTarget(target);
  return true;
}
