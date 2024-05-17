bool BrowserViewRenderer::CompositeSW(SkCanvas* canvas) {
  DCHECK(compositor_);
  CancelFallbackTick();
  ReturnResourceFromParent();
  return compositor_->DemandDrawSw(canvas);
}
