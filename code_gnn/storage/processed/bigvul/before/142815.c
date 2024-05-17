Node::InsertionNotificationRequest HTMLMediaElement::InsertedInto(
    ContainerNode& insertion_point) {
  BLINK_MEDIA_LOG << "insertedInto(" << (void*)this << ", " << insertion_point
                  << ")";

  HTMLElement::InsertedInto(insertion_point);
  if (insertion_point.isConnected()) {
    UseCounter::Count(GetDocument(), WebFeature::kHTMLMediaElementInDocument);
    if ((!getAttribute(kSrcAttr).IsEmpty() || src_object_) &&
        network_state_ == kNetworkEmpty) {
      ignore_preload_none_ = false;
      InvokeLoadAlgorithm();
    }
  }

  return kInsertionShouldCallDidNotifySubtreeInsertions;
}
