void PaintPropertyTreeBuilder::UpdateCompositedLayerPaginationOffset() {
  if (RuntimeEnabledFeatures::SlimmingPaintV2Enabled())
    return;

  const auto* enclosing_pagination_layer =
      context_.painting_layer->EnclosingPaginationLayer();
  if (!enclosing_pagination_layer)
    return;

  FragmentData& first_fragment =
      object_.GetMutableForPainting().FirstFragment();
  bool is_paint_invalidation_container = object_.IsPaintInvalidationContainer();
  const auto* parent_composited_layer =
      context_.painting_layer->EnclosingLayerWithCompositedLayerMapping(
          is_paint_invalidation_container ? kExcludeSelf : kIncludeSelf);
  if (is_paint_invalidation_container &&
      (!parent_composited_layer ||
       !parent_composited_layer->EnclosingPaginationLayer())) {
    FragmentainerIterator iterator(
        ToLayoutFlowThread(enclosing_pagination_layer->GetLayoutObject()),
        BoundingBoxInPaginationContainer(object_, *enclosing_pagination_layer));
    if (!iterator.AtEnd()) {
      first_fragment.SetPaginationOffset(
          ToLayoutPoint(iterator.PaginationOffset()));
      first_fragment.SetLogicalTopInFlowThread(
          iterator.FragmentainerLogicalTopInFlowThread());
    }
  } else if (parent_composited_layer) {
    const auto& fragment =
        parent_composited_layer->GetLayoutObject().FirstFragment();
    first_fragment.SetPaginationOffset(fragment.PaginationOffset());
    first_fragment.SetLogicalTopInFlowThread(fragment.LogicalTopInFlowThread());
  }
}
