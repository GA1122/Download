bool HTMLFormControlElement::checkValidity(WillBeHeapVector<RefPtrWillBeMember<HTMLFormControlElement>>* unhandledInvalidControls, CheckValidityEventBehavior eventBehavior)
{
    if (isValidElement())
        return true;
    if (eventBehavior != CheckValidityDispatchInvalidEvent)
        return false;
    RefPtrWillBeRawPtr<HTMLFormControlElement> protector(this);
    RefPtrWillBeRawPtr<Document> originalDocument(document());
    bool needsDefaultAction = dispatchEvent(Event::createCancelable(EventTypeNames::invalid));
    if (needsDefaultAction && unhandledInvalidControls && inDocument() && originalDocument == document())
        unhandledInvalidControls->append(this);
    return false;
}
