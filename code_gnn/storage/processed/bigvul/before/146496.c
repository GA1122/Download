void WebGLRenderingContextBase::frontFace(GLenum mode) {
  if (isContextLost())
    return;
  ContextGL()->FrontFace(mode);
}
