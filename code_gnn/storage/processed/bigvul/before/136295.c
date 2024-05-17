static LayoutRect MapLocalRectToAncestorLayer(
    const LayoutBox& box,
    const LayoutRect& local_rect,
    const PaintLayer& ancestor_layer) {
  TransformState transform_state(TransformState::kApplyTransformDirection,
                                 FloatPoint(local_rect.Location()));
  box.MapLocalToAncestor(&ancestor_layer.GetLayoutObject(), transform_state,
                         kApplyContainerFlip);
  transform_state.Flatten();
  return LayoutRect(LayoutPoint(transform_state.LastPlanarPoint()),
                    local_rect.Size());
}
