void PaintLayerScrollableArea::InvalidateAllStickyConstraints() {
  if (PaintLayerScrollableAreaRareData* d = RareData()) {
    for (PaintLayer* sticky_layer : d->sticky_constraints_map_.Keys()) {
      if (sticky_layer->GetLayoutObject().StyleRef().GetPosition() ==
          EPosition::kSticky) {
        sticky_layer->SetNeedsCompositingInputsUpdate();
        sticky_layer->GetLayoutObject().SetNeedsPaintPropertyUpdate();
      }
    }
    d->sticky_constraints_map_.clear();
  }
}
