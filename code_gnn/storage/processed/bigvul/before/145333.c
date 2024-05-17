bool isEnterKeyKeydownEvent(Event* event)
{
    return event->type() == EventTypeNames::keydown && event->isKeyboardEvent() && toKeyboardEvent(event)->keyIdentifier() == "Enter" && !toKeyboardEvent(event)->repeat();
}
