Node* TextTrackCue::Owner() const {
  return track_ ? track_->Owner() : nullptr;
}
