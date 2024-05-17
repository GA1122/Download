bool PaintLayerScrollableArea::HasNonCompositedStickyDescendants() const {
  if (const PaintLayerScrollableAreaRareData* d = RareData()) {
    for (const PaintLayer* sticky_layer : d->sticky_constraints_map_.Keys()) {
      if (sticky_layer->GetLayoutObject().IsSlowRepaintConstrainedObject())
        return true;
    }
  }
  return false;
}
