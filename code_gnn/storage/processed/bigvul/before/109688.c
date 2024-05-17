EventListener* Document::getWindowAttributeEventListener(const AtomicString& eventType, DOMWrapperWorld* isolatedWorld)
{
    DOMWindow* domWindow = this->domWindow();
    if (!domWindow)
        return 0;
    return domWindow->getAttributeEventListener(eventType, isolatedWorld);
}
