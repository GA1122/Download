MediaControlCastButtonElement* MediaControlCastButtonElement::create(
    MediaControls& mediaControls,
    bool isOverlayButton) {
  MediaControlCastButtonElement* button =
      new MediaControlCastButtonElement(mediaControls, isOverlayButton);
  button->ensureUserAgentShadowRoot();
  button->setType(InputTypeNames::button);
  return button;
}
