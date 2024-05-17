MediaControlOverflowMenuListElement::create(MediaControls& mediaControls) {
  MediaControlOverflowMenuListElement* element =
      new MediaControlOverflowMenuListElement(mediaControls);
  element->setIsWanted(false);
  element->setShadowPseudoId(
      AtomicString("-internal-media-controls-overflow-menu-list"));
  return element;
}
