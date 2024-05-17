void PaintPropertyTreeBuilder::UpdatePaintingLayer() {
  bool changed_painting_layer = false;
  if (object_.HasLayer() &&
      ToLayoutBoxModelObject(object_).HasSelfPaintingLayer()) {
    context_.painting_layer = ToLayoutBoxModelObject(object_).Layer();
    changed_painting_layer = true;
  } else if (object_.IsColumnSpanAll() ||
             object_.IsFloatingWithNonContainingBlockParent()) {
    context_.painting_layer = object_.PaintingLayer();
    changed_painting_layer = true;
  }
  DCHECK(context_.painting_layer == object_.PaintingLayer());
}
