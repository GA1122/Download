MediaControlTimeRemainingDisplayElement::create(MediaControls& mediaControls) {
  MediaControlTimeRemainingDisplayElement* element =
      new MediaControlTimeRemainingDisplayElement(mediaControls);
  element->setShadowPseudoId(
      AtomicString("-webkit-media-controls-time-remaining-display"));
  return element;
}
