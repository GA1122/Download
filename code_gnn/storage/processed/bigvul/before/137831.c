MediaControlMuteButtonElement* MediaControlMuteButtonElement::create(
    MediaControls& mediaControls) {
  MediaControlMuteButtonElement* button =
      new MediaControlMuteButtonElement(mediaControls);
  button->ensureUserAgentShadowRoot();
  button->setType(InputTypeNames::button);
  button->setShadowPseudoId(AtomicString("-webkit-media-controls-mute-button"));
  return button;
}
