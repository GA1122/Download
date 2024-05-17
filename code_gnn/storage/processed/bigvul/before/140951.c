void Document::AdjustFloatQuadsForScrollAndAbsoluteZoom(
    Vector<FloatQuad>& quads,
    const LayoutObject& layout_object) const {
  if (!View())
    return;

  for (wtf_size_t i = 0; i < quads.size(); ++i) {
    AdjustForAbsoluteZoom::AdjustFloatQuad(quads[i], layout_object);
  }
}
