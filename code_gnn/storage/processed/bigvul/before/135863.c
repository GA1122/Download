bool TextTrack::CanBeRendered() const {
  return GetReadinessState() != kFailedToLoad && IsVisualKind();
}
