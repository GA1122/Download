bool GeometryMapper::SlowLocalToAncestorVisualRectWithEffects(
    const PropertyTreeState& local_state,
    const PropertyTreeState& ancestor_state,
    FloatClipRect& mapping_rect,
    OverlayScrollbarClipBehavior clip_behavior,
    InclusiveIntersectOrNot inclusive_behavior,
    bool& success) {
  PropertyTreeState last_transform_and_clip_state(local_state.Transform(),
                                                  local_state.Clip(), nullptr);

  for (const auto* effect = local_state.Effect();
       effect && effect != ancestor_state.Effect(); effect = effect->Parent()) {
    if (!effect->HasFilterThatMovesPixels())
      continue;

    DCHECK(effect->OutputClip());
    PropertyTreeState transform_and_clip_state(effect->LocalTransformSpace(),
                                               effect->OutputClip(), nullptr);
    bool intersects = LocalToAncestorVisualRectInternal(
        last_transform_and_clip_state, transform_and_clip_state, mapping_rect,
        clip_behavior, inclusive_behavior, success);
    if (!success || !intersects) {
      success = true;
      mapping_rect = FloatClipRect(FloatRect());
      return false;
    }

    mapping_rect = FloatClipRect(effect->MapRect(mapping_rect.Rect()));
    last_transform_and_clip_state = transform_and_clip_state;
  }

  PropertyTreeState final_transform_and_clip_state(
      ancestor_state.Transform(), ancestor_state.Clip(), nullptr);
  LocalToAncestorVisualRectInternal(
      last_transform_and_clip_state, final_transform_and_clip_state,
      mapping_rect, clip_behavior, inclusive_behavior, success);

  mapping_rect.ClearIsTight();
  return true;
}
