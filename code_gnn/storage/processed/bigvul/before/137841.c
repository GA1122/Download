MediaControlFullscreenButtonElement::create(MediaControls& mediaControls) {
  MediaControlFullscreenButtonElement* button =
      new MediaControlFullscreenButtonElement(mediaControls);
  button->ensureUserAgentShadowRoot();
  button->setType(InputTypeNames::button);
  button->setShadowPseudoId(
      AtomicString("-webkit-media-controls-fullscreen-button"));
  button->setIsFullscreen(mediaControls.mediaElement().isFullscreen());
  button->setIsWanted(false);
  return button;
}
