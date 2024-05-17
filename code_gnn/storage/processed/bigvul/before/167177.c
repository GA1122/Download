void HTMLMediaElement::SelectInitialTracksIfNecessary() {
  if (!MediaTracksEnabledInternally())
    return;

  if (audioTracks().length() > 0 && !audioTracks().HasEnabledTrack())
    audioTracks().AnonymousIndexedGetter(0)->setEnabled(true);

  if (videoTracks().length() > 0 && videoTracks().selectedIndex() == -1)
    videoTracks().AnonymousIndexedGetter(0)->setSelected(true);
}
