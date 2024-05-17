bool TextTrack::IsRendered() const {
  return mode_ == ShowingKeyword() && IsVisualKind();
}
