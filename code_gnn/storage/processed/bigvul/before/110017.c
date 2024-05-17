Node::InsertionNotificationRequest HTMLSelectElement::insertedInto(ContainerNode* insertionPoint)
{
    recalcListItems();
    HTMLFormControlElementWithState::insertedInto(insertionPoint);
    return InsertionDone;
}
