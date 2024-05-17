Node::InsertionNotificationRequest HTMLInputElement::insertedInto(ContainerNode* insertionPoint)
{
    HTMLTextFormControlElement::insertedInto(insertionPoint);
    if (insertionPoint->inDocument() && !form())
        addToRadioButtonGroup();
    resetListAttributeTargetObserver();
    return InsertionDone;
}
