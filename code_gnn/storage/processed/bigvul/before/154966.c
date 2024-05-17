void WebGLRenderingContextBase::MarkLayerComposited() {
  if (!isContextLost())
    GetDrawingBuffer()->ResetBuffersToAutoClear();
}
