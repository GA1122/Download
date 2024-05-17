bool GeometryMapper::LocalToAncestorVisualRectInternal(
    const PropertyTreeState& local_state,
    const PropertyTreeState& ancestor_state,
    FloatClipRect& rect_to_map,
    OverlayScrollbarClipBehavior clip_behavior,
    InclusiveIntersectOrNot inclusive_behavior,
    bool& success) {
  if (local_state == ancestor_state) {
    success = true;
    return true;
  }

  if (local_state.Effect() != ancestor_state.Effect()) {
    return SlowLocalToAncestorVisualRectWithEffects(
        local_state, ancestor_state, rect_to_map, clip_behavior,
        inclusive_behavior, success);
  }

  const auto& transform_matrix = SourceToDestinationProjectionInternal(
      local_state.Transform(), ancestor_state.Transform(), success);
  if (!success) {
    success = true;
    rect_to_map = FloatClipRect(FloatRect());
    return false;
  }
  rect_to_map.Map(transform_matrix);

  FloatClipRect clip_rect = LocalToAncestorClipRectInternal(
      local_state.Clip(), ancestor_state.Clip(), ancestor_state.Transform(),
      clip_behavior, inclusive_behavior, success);
  if (success) {
    if (inclusive_behavior == kInclusiveIntersect)
      return rect_to_map.InclusiveIntersect(clip_rect);
    rect_to_map.Intersect(clip_rect);
    return !rect_to_map.Rect().IsEmpty();
  }

  if (!RuntimeEnabledFeatures::SlimmingPaintV2Enabled()) {
    success = true;
    rect_to_map.ClearIsTight();
  }
  return !rect_to_map.Rect().IsEmpty();
}
