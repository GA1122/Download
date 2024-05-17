void WebGLRenderingContextBase::bindBuffer(GLenum target, WebGLBuffer* buffer) {
  if (!ValidateNullableWebGLObject("bindBuffer", buffer))
    return;
  if (!ValidateAndUpdateBufferBindTarget("bindBuffer", target, buffer))
    return;
  ContextGL()->BindBuffer(target, ObjectOrZero(buffer));
}
