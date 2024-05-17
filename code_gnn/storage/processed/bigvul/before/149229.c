Node::InsertionNotificationRequest HTMLFormControlElement::InsertedInto(
    ContainerNode& insertion_point) {
  HTMLElement::InsertedInto(insertion_point);
  ListedElement::InsertedInto(insertion_point);

  if (ShouldAutofocus(this))
    GetDocument().TopDocument().EnqueueAutofocusCandidate(*this);
  return kInsertionDone;
}
