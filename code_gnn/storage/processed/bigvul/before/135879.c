HTMLMediaElement* TextTrack::MediaElement() const {
  return track_list_ ? track_list_->Owner() : nullptr;
}
