void WebGLRenderingContextBase::MarkCompositedAndClearBackbufferIfNeeded() {
  MarkLayerComposited();
  ClearIfComposited();
}
