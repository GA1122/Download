bool BrowserViewRenderer::OnDrawSoftware(SkCanvas* canvas) {
  return CanOnDraw() && CompositeSW(canvas);
}
