MediaControlCurrentTimeDisplayElement::create(MediaControls& mediaControls) {
  MediaControlCurrentTimeDisplayElement* element =
      new MediaControlCurrentTimeDisplayElement(mediaControls);
  element->setShadowPseudoId(
      AtomicString("-webkit-media-controls-current-time-display"));
  return element;
}
