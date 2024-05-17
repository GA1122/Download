MediaControlTextTrackListElement* MediaControlTextTrackListElement::create(
    MediaControls& mediaControls) {
  MediaControlTextTrackListElement* element =
      new MediaControlTextTrackListElement(mediaControls);
  element->setShadowPseudoId(
      AtomicString("-internal-media-controls-text-track-list"));
  element->setIsWanted(false);
  return element;
}
