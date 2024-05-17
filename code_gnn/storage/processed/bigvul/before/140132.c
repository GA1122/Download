bool HTMLMediaElement::textTracksAreReady() const {
  for (const auto& textTrack : m_textTracksWhenResourceSelectionBegan) {
    if (textTrack->getReadinessState() == TextTrack::Loading ||
        textTrack->getReadinessState() == TextTrack::NotLoaded)
      return false;
  }

  return true;
}
