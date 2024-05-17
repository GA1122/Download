void Document::setWindowAttributeEventListener(const AtomicString& eventType, PassRefPtrWillBeRawPtr<EventListener> listener)
{
    LocalDOMWindow* domWindow = this->domWindow();
    if (!domWindow)
        return;
    domWindow->setAttributeEventListener(eventType, listener);
}
