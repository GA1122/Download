void KeyboardClickableInputTypeView::handleKeyupEvent(KeyboardEvent* event)
{
    const String& key = event->keyIdentifier();
    if (key != "U+0020")
        return;
    dispatchSimulatedClickIfActive(event);
}
