MediaControlOverlayEnclosureElement::create(MediaControls& mediaControls) {
  MediaControlOverlayEnclosureElement* enclosure =
      new MediaControlOverlayEnclosureElement(mediaControls);
  enclosure->setShadowPseudoId(
      AtomicString("-webkit-media-controls-overlay-enclosure"));
  return enclosure;
}
