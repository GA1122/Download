TextTrackCueList* TextTrack::activeCues() {
  if (!cues_ || mode_ == DisabledKeyword())
    return nullptr;

  if (!active_cues_) {
    active_cues_ = TextTrackCueList::Create();
  }

  cues_->CollectActiveCues(*active_cues_);
  return active_cues_;
}
