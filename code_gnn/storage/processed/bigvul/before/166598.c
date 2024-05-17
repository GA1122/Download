void WebGL2RenderingContextBase::bindBufferBase(GLenum target,
                                                GLuint index,
                                                WebGLBuffer* buffer) {
  if (isContextLost())
    return;
  bool deleted;
  if (!CheckObjectToBeBound("bindBufferBase", buffer, deleted))
    return;
  if (deleted) {
    SynthesizeGLError(GL_INVALID_OPERATION, "bindBufferBase",
                      "attempt to bind a deleted buffer");
    return;
  }
  if (target == GL_TRANSFORM_FEEDBACK_BUFFER &&
      transform_feedback_binding_->active()) {
    SynthesizeGLError(GL_INVALID_OPERATION, "bindBufferBase",
                      "transform feedback is active");
    return;
  }
  if (!ValidateAndUpdateBufferBindBaseTarget("bindBufferBase", target, index,
                                             buffer))
    return;

  ContextGL()->BindBufferBase(target, index, ObjectOrZero(buffer));
}
