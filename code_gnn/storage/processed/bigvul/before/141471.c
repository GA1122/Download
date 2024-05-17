IntRect PaintLayerScrollableArea::ResizerCornerRect(
    const IntRect& bounds,
    ResizerHitTestType resizer_hit_test_type) const {
  if (GetLayoutBox()->StyleRef().Resize() == EResize::kNone)
    return IntRect();
  IntRect corner = CornerRect(bounds);

  if (resizer_hit_test_type == kResizerForTouch) {
    int expand_ratio = kResizerControlExpandRatioForTouch - 1;
    corner.Move(-corner.Width() * expand_ratio,
                -corner.Height() * expand_ratio);
    corner.Expand(corner.Width() * expand_ratio,
                  corner.Height() * expand_ratio);
  }

  return corner;
}
