void KeyboardClickableInputTypeView::handleKeydownEvent(KeyboardEvent* event)
{
    const String& key = event->keyIdentifier();
    if (key == "U+0020") {
        element().setActive(true);
    }
}
