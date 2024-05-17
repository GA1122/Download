IntSize PaintLayerScrollableArea::MinimumScrollOffsetInt() const {
  return ToIntSize(-ScrollOrigin());
}
