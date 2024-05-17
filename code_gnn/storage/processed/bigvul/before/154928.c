void WebGLRenderingContextBase::DidDraw(const SkIRect& dirty_rect) {
  MarkContextChanged(kCanvasChanged);
  CanvasRenderingContext::DidDraw(dirty_rect);
}
