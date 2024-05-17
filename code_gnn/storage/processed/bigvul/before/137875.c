void MediaControlTextTrackListElement::refreshTextTrackListMenu() {
  if (!mediaElement().hasClosedCaptions() ||
      !mediaElement().textTracksAreReady())
    return;

  EventDispatchForbiddenScope::AllowUserAgentEvents allowEvents;
  removeChildren(OmitSubtreeModifiedEvent);

  appendChild(createTextTrackListItem(nullptr));

  TextTrackList* trackList = mediaElement().textTracks();
  for (unsigned i = 0; i < trackList->length(); i++) {
    TextTrack* track = trackList->anonymousIndexedGetter(i);
    if (!track->canBeRendered())
      continue;
    appendChild(createTextTrackListItem(track));
  }
}
