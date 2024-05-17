void Document::setWindowAttributeEventListener(const AtomicString& eventType, EventListener* listener)
{
    LocalDOMWindow* domWindow = this->domWindow();
    if (!domWindow)
        return;
    domWindow->setAttributeEventListener(eventType, listener);
}
