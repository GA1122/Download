void WebGLRenderingContextBase::finish() {
  if (isContextLost())
    return;
  ContextGL()->Flush();   
}
