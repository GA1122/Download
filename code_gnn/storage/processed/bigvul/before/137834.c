MediaControlToggleClosedCaptionsButtonElement::create(
    MediaControls& mediaControls) {
  MediaControlToggleClosedCaptionsButtonElement* button =
      new MediaControlToggleClosedCaptionsButtonElement(mediaControls);
  button->ensureUserAgentShadowRoot();
  button->setType(InputTypeNames::button);
  button->setShadowPseudoId(
      AtomicString("-webkit-media-controls-toggle-closed-captions-button"));
  button->setIsWanted(false);
  return button;
}
