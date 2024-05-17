static LayoutPoint VisualOffsetFromPaintOffsetRoot(
    const PaintPropertyTreeBuilderFragmentContext& context,
    const PaintLayer* child) {
  const LayoutObject* paint_offset_root = context.current.paint_offset_root;
  PaintLayer* painting_layer = paint_offset_root->PaintingLayer();
  LayoutPoint result = child->VisualOffsetFromAncestor(painting_layer);
  if (!paint_offset_root->HasLayer() ||
      ToLayoutBoxModelObject(paint_offset_root)->Layer() != painting_layer) {
    result.Move(-paint_offset_root->OffsetFromAncestorContainer(
        &painting_layer->GetLayoutObject()));
  }

  if (paint_offset_root->HasOverflowClip())
    result += ToLayoutBox(paint_offset_root)->ScrolledContentOffset();
  return result;
}
