IntSize PaintLayerScrollableArea::PixelSnappedContentsSize(
    const LayoutPoint& paint_offset) const {
  return PixelSnappedIntSize(overflow_rect_.Size(), paint_offset);
}
