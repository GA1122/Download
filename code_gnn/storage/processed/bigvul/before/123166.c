gfx::Rect RenderWidgetHostViewAura::GetCaretBounds() {
  const gfx::Rect rect =
      gfx::UnionRects(selection_start_rect_, selection_end_rect_);
  return ConvertRectToScreen(rect);
}
