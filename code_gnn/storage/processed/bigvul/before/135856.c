static bool EventTimeCueCompare(const std::pair<double, TextTrackCue*>& a,
                                const std::pair<double, TextTrackCue*>& b) {
  if (a.first != b.first)
    return a.first - b.first < 0;

  if (a.second->track() != b.second->track())
    return TrackIndexCompare(a.second->track(), b.second->track());

  return a.second->CueIndex() < b.second->CueIndex();
}
