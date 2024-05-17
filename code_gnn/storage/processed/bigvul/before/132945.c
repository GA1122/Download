gfx::Rect RenderWidgetHostViewAura::GetCaretBounds() const {
  const gfx::Rect rect =
      gfx::UnionRects(selection_anchor_rect_, selection_focus_rect_);
  return ConvertRectToScreen(rect);
}
