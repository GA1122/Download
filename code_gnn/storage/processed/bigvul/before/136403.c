static bool SkipGroupIfEffectivelyInvisible(
    const PaintArtifact& paint_artifact,
    const EffectPaintPropertyNode& current_group,
    Vector<PaintChunk>::const_iterator& chunk_it) {
  static const float kMinimumVisibleOpacity = 0.0004f;
  if (current_group.Opacity() >= kMinimumVisibleOpacity ||
      current_group.HasDirectCompositingReasons()) {
    return false;
  }

  DCHECK(EffectGroupContainsChunk(current_group, *chunk_it));
  while (++chunk_it != paint_artifact.PaintChunks().end()) {
    if (!EffectGroupContainsChunk(current_group, *chunk_it))
      break;
  }
  return true;
}
