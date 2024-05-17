void WebGLRenderingContextBase::clearStencil(GLint s) {
  if (isContextLost())
    return;
  clear_stencil_ = s;
  ContextGL()->ClearStencil(s);
}
