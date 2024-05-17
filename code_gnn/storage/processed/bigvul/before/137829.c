MediaControlPanelEnclosureElement* MediaControlPanelEnclosureElement::create(
    MediaControls& mediaControls) {
  MediaControlPanelEnclosureElement* enclosure =
      new MediaControlPanelEnclosureElement(mediaControls);
  enclosure->setShadowPseudoId(
      AtomicString("-webkit-media-controls-enclosure"));
  return enclosure;
}
