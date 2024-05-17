void PaintLayerScrollableArea::DidScroll(const FloatPoint& position) {
  ScrollableArea::DidScroll(position);
  CHECK(!HasBeenDisposed());
}
