inline PassRefPtrWillBeRawPtr<Event> createScriptLoadEvent()
{
    return Event::create(EventTypeNames::load);
}
