IntSize PaintLayerScrollableArea::ContentsSize() const {
  LayoutPoint offset(
      GetLayoutBox()->ClientLeft() + GetLayoutBox()->Location().X(),
      GetLayoutBox()->ClientTop() + GetLayoutBox()->Location().Y());
  return PixelSnappedContentsSize(offset);
}
