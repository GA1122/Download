bool Notification::dispatchEventInternal(PassRefPtrWillBeRawPtr<Event> event)
{
    ASSERT(executionContext()->isContextThread());
    return EventTarget::dispatchEventInternal(event);
}
