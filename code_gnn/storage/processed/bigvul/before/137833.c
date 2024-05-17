MediaControlOverlayPlayButtonElement::create(MediaControls& mediaControls) {
  MediaControlOverlayPlayButtonElement* button =
      new MediaControlOverlayPlayButtonElement(mediaControls);
  button->ensureUserAgentShadowRoot();
  button->setType(InputTypeNames::button);
  button->setShadowPseudoId(
      AtomicString("-webkit-media-controls-overlay-play-button"));
  return button;
}
