void BrowserWindowGtk::SetBounds(const gfx::Rect& bounds) {
  if (IsFullscreen())
    ExitFullscreen();
  SetBoundsImpl(bounds, true, true);
}
