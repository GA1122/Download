void HTMLMediaElement::MarkCaptionAndSubtitleTracksAsUnconfigured() {
  if (!text_tracks_)
    return;

  for (unsigned i = 0; i < text_tracks_->length(); ++i) {
    TextTrack* text_track = text_tracks_->AnonymousIndexedGetter(i);
    if (text_track->IsVisualKind())
      text_track->SetHasBeenConfigured(false);
  }
}
