void Document::AdjustFloatRectForScrollAndAbsoluteZoom(
    FloatRect& rect,
    const LayoutObject& layout_object) const {
  if (!View())
    return;

  LayoutRect visible_content_rect(View()->VisibleContentRect());
  rect.Move(-FloatSize(visible_content_rect.X().ToFloat(),
                       visible_content_rect.Y().ToFloat()));
  AdjustForAbsoluteZoom::AdjustFloatRect(rect, layout_object);
}
