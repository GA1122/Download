void WebGLRenderingContextBase::DidDraw() {
  MarkContextChanged(kCanvasChanged);
  CanvasRenderingContext::DidDraw();
}
