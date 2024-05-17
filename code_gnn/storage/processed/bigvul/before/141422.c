bool PaintLayerScrollableArea::HasStickyDescendants() const {
  if (const PaintLayerScrollableAreaRareData* d = RareData())
    return !d->sticky_constraints_map_.IsEmpty();
  return false;
}
