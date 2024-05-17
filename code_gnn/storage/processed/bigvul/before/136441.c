static bool CombineClip(const ClipPaintPropertyNode* clip,
                        FloatRoundedRect& combined_clip_rect) {
  if (clip->Parent()->ClipPath())
    return false;

  if (clip->LocalTransformSpace() != clip->Parent()->LocalTransformSpace() &&
      !GeometryMapper::SourceToDestinationProjection(
           clip->Parent()->LocalTransformSpace(), clip->LocalTransformSpace())
           .IsIdentity())
    return false;

  bool clip_is_rounded = clip->ClipRect().IsRounded();
  bool combined_is_rounded = combined_clip_rect.IsRounded();
  if (clip_is_rounded && combined_is_rounded)
    return false;

  if (combined_is_rounded)
    return clip->ClipRect().Rect().Contains(combined_clip_rect.Rect());
  if (clip_is_rounded) {
    if (combined_clip_rect.Rect().Contains(clip->ClipRect().Rect())) {
      combined_clip_rect = clip->ClipRect();
      return true;
    }
    return false;
  }

  DCHECK(!combined_is_rounded && !clip_is_rounded);
  combined_clip_rect = FloatRoundedRect(
      Intersection(combined_clip_rect.Rect(), clip->ClipRect().Rect()));
  return true;
}
