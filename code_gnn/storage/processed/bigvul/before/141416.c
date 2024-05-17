SmoothScrollSequencer* PaintLayerScrollableArea::GetSmoothScrollSequencer()
    const {
  if (HasBeenDisposed())
    return nullptr;

  return &GetLayoutBox()->GetFrame()->GetSmoothScrollSequencer();
}
