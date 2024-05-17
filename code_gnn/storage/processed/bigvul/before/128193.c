void Notification::dispatchShowEvent()
{
    dispatchEvent(Event::create(EventTypeNames::show));
}
