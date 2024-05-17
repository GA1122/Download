void WebGLRenderingContextBase::cullFace(GLenum mode) {
  if (isContextLost())
    return;
  ContextGL()->CullFace(mode);
}
