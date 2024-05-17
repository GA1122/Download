void WebGLRenderingContextBase::polygonOffset(GLfloat factor, GLfloat units) {
  if (isContextLost())
    return;
  ContextGL()->PolygonOffset(factor, units);
}
