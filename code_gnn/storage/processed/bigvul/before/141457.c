IntSize PaintLayerScrollableArea::OffsetFromResizeCorner(
    const IntPoint& absolute_point) const {
  IntSize element_size = Layer()->PixelSnappedSize();
  if (GetLayoutBox()->ShouldPlaceBlockDirectionScrollbarOnLogicalLeft())
    element_size.SetWidth(0);
  IntPoint resizer_point = IntPoint(element_size);
  IntPoint local_point = RoundedIntPoint(GetLayoutBox()->AbsoluteToLocal(
      FloatPoint(absolute_point), kUseTransforms));
  return local_point - resizer_point;
}
