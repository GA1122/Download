void HTMLMediaElement::selectInitialTracksIfNecessary() {
  if (!mediaTracksEnabledInternally())
    return;

  if (audioTracks().length() > 0 && !audioTracks().hasEnabledTrack())
    audioTracks().anonymousIndexedGetter(0)->setEnabled(true);

  if (videoTracks().length() > 0 && videoTracks().selectedIndex() == -1)
    videoTracks().anonymousIndexedGetter(0)->setSelected(true);
}
