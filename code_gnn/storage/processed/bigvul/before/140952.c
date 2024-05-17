void Document::AdjustFloatRectForScrollAndAbsoluteZoom(
    FloatRect& rect,
    const LayoutObject& layout_object) const {
  if (!View())
    return;

  AdjustForAbsoluteZoom::AdjustFloatRect(rect, layout_object);
}
