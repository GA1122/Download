FloatClipRect GeometryMapper::LocalToAncestorClipRectInternal(
    const ClipPaintPropertyNode* descendant,
    const ClipPaintPropertyNode* ancestor_clip,
    const TransformPaintPropertyNode* ancestor_transform,
    OverlayScrollbarClipBehavior clip_behavior,
    InclusiveIntersectOrNot inclusive_behavior,
    bool& success) {
  if (descendant == ancestor_clip) {
    success = true;
    return FloatClipRect();
  }

  if (descendant->Parent() == ancestor_clip &&
      descendant->LocalTransformSpace() == ancestor_transform) {
    success = true;
    return GetClipRect(descendant, clip_behavior);
  }

  FloatClipRect clip;
  const ClipPaintPropertyNode* clip_node = descendant;
  Vector<const ClipPaintPropertyNode*> intermediate_nodes;

  GeometryMapperClipCache::ClipAndTransform clip_and_transform(
      ancestor_clip, ancestor_transform, clip_behavior);
  while (clip_node && clip_node != ancestor_clip) {
    const FloatClipRect* cached_clip = nullptr;
    if (inclusive_behavior != kInclusiveIntersect)
      cached_clip = clip_node->GetClipCache().GetCachedClip(clip_and_transform);

    if (cached_clip) {
      clip = *cached_clip;
      break;
    }

    intermediate_nodes.push_back(clip_node);
    clip_node = clip_node->Parent();
  }
  if (!clip_node) {
    success = false;
    if (!RuntimeEnabledFeatures::SlimmingPaintV2Enabled()) {
      success = true;
    }
    FloatClipRect loose_infinite;
    loose_infinite.ClearIsTight();
    return loose_infinite;
  }

  for (auto it = intermediate_nodes.rbegin(); it != intermediate_nodes.rend();
       ++it) {
    const TransformationMatrix& transform_matrix =
        SourceToDestinationProjectionInternal((*it)->LocalTransformSpace(),
                                              ancestor_transform, success);
    if (!success) {
      success = true;
      return FloatClipRect(FloatRect());
    }

    FloatClipRect mapped_rect(GetClipRect((*it), clip_behavior));
    mapped_rect.Map(transform_matrix);
    if (inclusive_behavior == kInclusiveIntersect) {
      clip.InclusiveIntersect(mapped_rect);
    } else {
      clip.Intersect(mapped_rect);
      (*it)->GetClipCache().SetCachedClip(clip_and_transform, clip);
    }
  }
  DCHECK(inclusive_behavior == kInclusiveIntersect ||
         *descendant->GetClipCache().GetCachedClip(clip_and_transform) == clip);
  success = true;
  return clip;
}
