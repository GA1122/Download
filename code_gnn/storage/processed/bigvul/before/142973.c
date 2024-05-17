TextTrackList* HTMLMediaElement::textTracks() {
  if (!text_tracks_)
    text_tracks_ = TextTrackList::Create(this);

  return text_tracks_.Get();
}
