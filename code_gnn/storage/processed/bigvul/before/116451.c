void BrowserNonClientFrameViewAura::OnPaint(gfx::Canvas* canvas) {
  if (frame()->IsFullscreen())
    return;   
  frame_painter_->PaintHeader(this,
                              canvas,
                              GetThemeFrameBitmap(),
                              GetThemeFrameOverlayBitmap());
  frame_painter_->PaintTitleBar(this, canvas, BrowserFrame::GetTitleFont());
  if (browser_view()->IsToolbarVisible())
    PaintToolbarBackground(canvas);
}
