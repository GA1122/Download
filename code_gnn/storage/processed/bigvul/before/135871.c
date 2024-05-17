CueTimeline* TextTrack::GetCueTimeline() const {
  return MediaElement() ? &MediaElement()->GetCueTimeline() : nullptr;
}
