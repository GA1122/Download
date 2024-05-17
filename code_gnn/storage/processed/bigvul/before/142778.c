void HTMLMediaElement::DidRemoveTrackElement(HTMLTrackElement* track_element) {
  KURL url = track_element->GetNonEmptyURLAttribute(kSrcAttr);
  BLINK_MEDIA_LOG << "didRemoveTrackElement(" << (void*)this << ") - 'src' is "
                  << UrlForLoggingMedia(url);

  TextTrack* text_track = track_element->track();
  if (!text_track)
    return;

  text_track->SetHasBeenConfigured(false);

  if (!text_tracks_)
    return;

  text_tracks_->Remove(text_track);

  wtf_size_t index =
      text_tracks_when_resource_selection_began_.Find(text_track);
  if (index != kNotFound)
    text_tracks_when_resource_selection_began_.EraseAt(index);
}
