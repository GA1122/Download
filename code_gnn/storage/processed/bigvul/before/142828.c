void HTMLMediaElement::LoadInternal() {
  text_tracks_when_resource_selection_began_.clear();
  if (text_tracks_) {
    for (unsigned i = 0; i < text_tracks_->length(); ++i) {
      TextTrack* track = text_tracks_->AnonymousIndexedGetter(i);
      if (track->mode() != TextTrack::DisabledKeyword())
        text_tracks_when_resource_selection_began_.push_back(track);
    }
  }

  SelectMediaResource();
}
