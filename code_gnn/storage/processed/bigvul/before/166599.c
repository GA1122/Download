void WebGL2RenderingContextBase::bindBufferRange(GLenum target,
                                                 GLuint index,
                                                 WebGLBuffer* buffer,
                                                 long long offset,
                                                 long long size) {
  if (isContextLost())
    return;
  bool deleted;
  if (!CheckObjectToBeBound("bindBufferRange", buffer, deleted))
    return;
  if (deleted) {
    SynthesizeGLError(GL_INVALID_OPERATION, "bindBufferRange",
                      "attempt to bind a deleted buffer");
    return;
  }
  if (target == GL_TRANSFORM_FEEDBACK_BUFFER &&
      transform_feedback_binding_->active()) {
    SynthesizeGLError(GL_INVALID_OPERATION, "bindBufferBase",
                      "transform feedback is active");
    return;
  }
  if (!ValidateValueFitNonNegInt32("bindBufferRange", "offset", offset) ||
      !ValidateValueFitNonNegInt32("bindBufferRange", "size", size)) {
    return;
  }

  if (!ValidateAndUpdateBufferBindBaseTarget("bindBufferRange", target, index,
                                             buffer))
    return;

  ContextGL()->BindBufferRange(target, index, ObjectOrZero(buffer),
                               static_cast<GLintptr>(offset),
                               static_cast<GLsizeiptr>(size));
}
