bool GeometryMapper::PointVisibleInAncestorSpace(
    const PropertyTreeState& local_state,
    const PropertyTreeState& ancestor_state,
    const FloatPoint& local_point) {
  for (const auto* clip = local_state.Clip();
       clip && clip != ancestor_state.Clip(); clip = clip->Parent()) {
    FloatPoint mapped_point =
        SourceToDestinationProjection(local_state.Transform(),
                                      clip->LocalTransformSpace())
            .MapPoint(local_point);

    if (!clip->ClipRect().IntersectsQuad(
            FloatRect(mapped_point, FloatSize(1, 1))))
      return false;

    if (clip->ClipPath() && !clip->ClipPath()->Contains(mapped_point))
      return false;
  }

  return true;
}
