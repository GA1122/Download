void HTMLMediaElement::didRemoveTrackElement(HTMLTrackElement* trackElement) {
  KURL url = trackElement->getNonEmptyURLAttribute(srcAttr);
  BLINK_MEDIA_LOG << "didRemoveTrackElement(" << (void*)this << ") - 'src' is "
                  << urlForLoggingMedia(url);

  TextTrack* textTrack = trackElement->track();
  if (!textTrack)
    return;

  textTrack->setHasBeenConfigured(false);

  if (!m_textTracks)
    return;

  m_textTracks->remove(textTrack);

  size_t index = m_textTracksWhenResourceSelectionBegan.find(textTrack);
  if (index != kNotFound)
    m_textTracksWhenResourceSelectionBegan.remove(index);
}
