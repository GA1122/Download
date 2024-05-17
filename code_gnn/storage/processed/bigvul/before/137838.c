MediaControlDownloadButtonElement* MediaControlDownloadButtonElement::create(
    MediaControls& mediaControls) {
  MediaControlDownloadButtonElement* button =
      new MediaControlDownloadButtonElement(mediaControls);
  button->ensureUserAgentShadowRoot();
  button->setType(InputTypeNames::button);
  button->setShadowPseudoId(
      AtomicString("-internal-media-controls-download-button"));
  button->setIsWanted(false);
  return button;
}
