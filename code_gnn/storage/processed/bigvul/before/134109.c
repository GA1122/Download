chromeos::ibus::Rect GfxRectToIBusRect(const gfx::Rect& rect) {
  return chromeos::ibus::Rect(rect.x(), rect.y(), rect.width(), rect.height());
}
