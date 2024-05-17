MediaControlPlayButtonElement* MediaControlPlayButtonElement::create(
    MediaControls& mediaControls) {
  MediaControlPlayButtonElement* button =
      new MediaControlPlayButtonElement(mediaControls);
  button->ensureUserAgentShadowRoot();
  button->setType(InputTypeNames::button);
  button->setShadowPseudoId(AtomicString("-webkit-media-controls-play-button"));
  return button;
}
