static LayoutPoint PaintOffsetInPaginationContainer(
    const LayoutObject& object,
    const PaintLayer& enclosing_pagination_layer) {
  if (!object.IsBox() && !object.HasLayer()) {
    return PaintOffsetInPaginationContainer(*object.ContainingBlock(),
                                            enclosing_pagination_layer);
  }

  TransformState transform_state(TransformState::kApplyTransformDirection,
                                 FloatPoint());
  object.MapLocalToAncestor(&enclosing_pagination_layer.GetLayoutObject(),
                            transform_state, kApplyContainerFlip);
  transform_state.Flatten();
  return LayoutPoint(transform_state.LastPlanarPoint());
}
