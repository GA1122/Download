void WebGLRenderingContextBase::bindRenderbuffer(
    GLenum target,
    WebGLRenderbuffer* render_buffer) {
  if (!ValidateNullableWebGLObject("bindRenderbuffer", render_buffer))
    return;
  if (target != GL_RENDERBUFFER) {
    SynthesizeGLError(GL_INVALID_ENUM, "bindRenderbuffer", "invalid target");
    return;
  }
  renderbuffer_binding_ = render_buffer;
  ContextGL()->BindRenderbuffer(target, ObjectOrZero(render_buffer));
  if (render_buffer)
    render_buffer->SetHasEverBeenBound();
}
