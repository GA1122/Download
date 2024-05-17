EventListener* Document::getWindowAttributeEventListener(const AtomicString& eventType)
{
    LocalDOMWindow* domWindow = this->domWindow();
    if (!domWindow)
        return 0;
    return domWindow->getAttributeEventListener(eventType);
}
