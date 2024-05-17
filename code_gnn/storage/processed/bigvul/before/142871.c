void HTMLMediaElement::RemovedFrom(ContainerNode& insertion_point) {
  BLINK_MEDIA_LOG << "removedFrom(" << (void*)this << ", " << insertion_point
                  << ")";

  removed_from_document_timer_.StartOneShot(TimeDelta(), FROM_HERE);

  HTMLElement::RemovedFrom(insertion_point);
}
