bool TrackIsInactive(const MediaStreamTrack& track) {
  return track.readyState() != "live" || !track.enabled() || track.muted();
}
