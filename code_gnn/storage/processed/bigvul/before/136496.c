bool GeometryMapper::LocalToAncestorVisualRect(
    const PropertyTreeState& local_state,
    const PropertyTreeState& ancestor_state,
    FloatClipRect& mapping_rect,
    OverlayScrollbarClipBehavior clip_behavior,
    InclusiveIntersectOrNot inclusive_behavior) {
  bool success = false;
  bool result = LocalToAncestorVisualRectInternal(local_state, ancestor_state,
                                                  mapping_rect, clip_behavior,
                                                  inclusive_behavior, success);
  DCHECK(success);
  return result;
}
