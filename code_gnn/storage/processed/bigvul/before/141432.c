void PaintLayerScrollableArea::InvalidatePaintForStickyDescendants() {
  if (PaintLayerScrollableAreaRareData* d = RareData()) {
    for (PaintLayer* sticky_layer : d->sticky_constraints_map_.Keys())
      sticky_layer->GetLayoutObject().SetNeedsPaintPropertyUpdate();
  }
}
