void WebGLRenderingContextBase::blendEquation(GLenum mode) {
  if (isContextLost() || !ValidateBlendEquation("blendEquation", mode))
    return;
  ContextGL()->BlendEquation(mode);
}
