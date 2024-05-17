bool PaintLayerScrollableArea::IsPointInResizeControl(
    const IntPoint& absolute_point,
    ResizerHitTestType resizer_hit_test_type) const {
  if (!GetLayoutBox()->CanResize())
    return false;

  IntPoint local_point = RoundedIntPoint(GetLayoutBox()->AbsoluteToLocal(
      FloatPoint(absolute_point), kUseTransforms));
  IntRect local_bounds(IntPoint(), Layer()->PixelSnappedSize());
  return ResizerCornerRect(local_bounds, resizer_hit_test_type)
      .Contains(local_point);
}
