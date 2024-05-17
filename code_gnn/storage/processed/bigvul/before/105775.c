gfx::Rect Label::GetAvailableRect() const {
  gfx::Rect bounds(gfx::Point(), size());
  gfx::Insets insets(GetInsets());
  bounds.Inset(insets.left(), insets.top(), insets.right(), insets.bottom());
  return bounds;
}
