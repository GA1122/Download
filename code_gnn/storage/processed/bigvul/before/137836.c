MediaControlOverflowMenuButtonElement::create(MediaControls& mediaControls) {
  MediaControlOverflowMenuButtonElement* button =
      new MediaControlOverflowMenuButtonElement(mediaControls);
  button->ensureUserAgentShadowRoot();
  button->setType(InputTypeNames::button);
  button->setShadowPseudoId(
      AtomicString("-internal-media-controls-overflow-button"));
  button->setIsWanted(false);
  return button;
}
