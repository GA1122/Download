void HTMLMediaElement::removedFrom(ContainerNode* insertionPoint) {
  BLINK_MEDIA_LOG << "removedFrom(" << (void*)this << ", " << insertionPoint
                  << ")";

  HTMLElement::removedFrom(insertionPoint);
  if (insertionPoint->inActiveDocument()) {
    updateControlsVisibility();
    if (m_networkState > kNetworkEmpty)
      pauseInternal();
  }
}
