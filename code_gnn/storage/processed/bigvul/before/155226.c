Node::InsertionNotificationRequest HTMLFormElement::insertedInto(
    ContainerNode* insertionPoint) {
  HTMLElement::insertedInto(insertionPoint);
  logAddElementIfIsolatedWorldAndInDocument("form", methodAttr, actionAttr);
  if (insertionPoint->isConnected())
    this->document().didAssociateFormControl(this);
  return InsertionDone;
}
