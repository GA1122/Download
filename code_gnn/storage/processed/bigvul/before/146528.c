void WebGLRenderingContextBase::lineWidth(GLfloat width) {
  if (isContextLost())
    return;
  ContextGL()->LineWidth(width);
}
