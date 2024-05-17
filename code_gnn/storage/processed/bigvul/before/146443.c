void WebGLRenderingContextBase::bindBuffer(GLenum target, WebGLBuffer* buffer) {
  bool deleted;
  if (!CheckObjectToBeBound("bindBuffer", buffer, deleted))
    return;
  if (deleted) {
    SynthesizeGLError(GL_INVALID_OPERATION, "bindBuffer",
                      "attempt to bind a deleted buffer");
    return;
  }
  if (!ValidateAndUpdateBufferBindTarget("bindBuffer", target, buffer))
    return;
  ContextGL()->BindBuffer(target, ObjectOrZero(buffer));
}
