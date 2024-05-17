Node::InsertionNotificationRequest HTMLInputElement::InsertedInto(
    ContainerNode* insertion_point) {
  TextControlElement::InsertedInto(insertion_point);
  if (insertion_point->isConnected() && !Form())
    AddToRadioButtonGroup();
  ResetListAttributeTargetObserver();
  LogAddElementIfIsolatedWorldAndInDocument("input", typeAttr, formactionAttr);
  return kInsertionShouldCallDidNotifySubtreeInsertions;
}
