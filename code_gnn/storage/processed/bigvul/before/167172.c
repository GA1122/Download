void HTMLMediaElement::RemovedFrom(ContainerNode* insertion_point) {
  BLINK_MEDIA_LOG << "removedFrom(" << (void*)this << ", " << insertion_point
                  << ")";

  HTMLElement::RemovedFrom(insertion_point);
  if (insertion_point->InActiveDocument()) {
    UpdateControlsVisibility();
    if (network_state_ > kNetworkEmpty)
      PauseInternal();
  }
}
