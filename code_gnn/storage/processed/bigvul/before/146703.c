void Document::AdjustFloatQuadsForScrollAndAbsoluteZoom(
    Vector<FloatQuad>& quads,
    LayoutObject& layout_object) {
  if (!View())
    return;

  LayoutRect visible_content_rect(View()->VisibleContentRect());
  for (size_t i = 0; i < quads.size(); ++i) {
    quads[i].Move(-FloatSize(visible_content_rect.X().ToFloat(),
                             visible_content_rect.Y().ToFloat()));
    AdjustFloatQuadForAbsoluteZoom(quads[i], layout_object);
  }
}
