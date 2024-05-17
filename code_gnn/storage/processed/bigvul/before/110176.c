 void DateTimeFieldElement::defaultEventHandler(Event* event)
{
    if (event->type() == eventNames().blurEvent)
        didBlur();

    if (event->type() == eventNames().focusEvent)
        didFocus();

    if (event->isKeyboardEvent()) {
        KeyboardEvent* keyboardEvent = static_cast<KeyboardEvent*>(event);
        handleKeyboardEvent(keyboardEvent);
        if (keyboardEvent->defaultHandled())
            return;
        defaultKeyboardEventHandler(keyboardEvent);
        if (keyboardEvent->defaultHandled())
            return;
    }

    HTMLElement::defaultEventHandler(event);
}
