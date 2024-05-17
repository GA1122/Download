bool isLinkClick(Event* event)
{
    return event->type() == EventTypeNames::click && (!event->isMouseEvent() || (toMouseEvent(event)->button() != RightButton && toMouseEvent(event)->detail() <= 1));
}
