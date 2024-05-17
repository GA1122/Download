Node::InsertionNotificationRequest HTMLFormElement::InsertedInto(
    ContainerNode* insertion_point) {
  HTMLElement::InsertedInto(insertion_point);
  LogAddElementIfIsolatedWorldAndInDocument("form", methodAttr, actionAttr);
  if (insertion_point->isConnected())
    GetDocument().DidAssociateFormControl(this);
  return kInsertionDone;
}
