void WebGLRenderingContextBase::viewport(GLint x,
                                         GLint y,
                                         GLsizei width,
                                         GLsizei height) {
  if (isContextLost())
    return;
  ContextGL()->Viewport(x, y, width, height);
}
