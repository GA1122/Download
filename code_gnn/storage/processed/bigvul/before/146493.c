void WebGLRenderingContextBase::flush() {
  if (isContextLost())
    return;
  ContextGL()->Flush();
}
