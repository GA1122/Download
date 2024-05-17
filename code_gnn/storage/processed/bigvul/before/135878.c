bool TextTrack::IsVisualKind() const {
  return kind() == SubtitlesKeyword() || kind() == CaptionsKeyword();
}
