Node::InsertionNotificationRequest HTMLMediaElement::insertedInto(
    ContainerNode* insertionPoint) {
  BLINK_MEDIA_LOG << "insertedInto(" << (void*)this << ", " << insertionPoint
                  << ")";

  HTMLElement::insertedInto(insertionPoint);
  if (insertionPoint->isConnected()) {
    UseCounter::count(document(), UseCounter::HTMLMediaElementInDocument);
    if ((!getAttribute(srcAttr).isEmpty() || m_srcObject) &&
        m_networkState == kNetworkEmpty) {
      m_ignorePreloadNone = false;
      invokeLoadAlgorithm();
    }
  }

  return InsertionShouldCallDidNotifySubtreeInsertions;
}
