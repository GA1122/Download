static PassOwnPtr<WebWheelEvent> coalesceWheelEvents(WebWheelEvent* oldNextWheelEvent, const WebWheelEvent& newWheelEvent)
{
#if MERGE_WHEEL_EVENTS
    if (!oldNextWheelEvent)
        return adoptPtr(new WebWheelEvent(newWheelEvent));

    if (oldNextWheelEvent->position() != newWheelEvent.position() || oldNextWheelEvent->modifiers() != newWheelEvent.modifiers() || oldNextWheelEvent->granularity() != newWheelEvent.granularity())
        return adoptPtr(new WebWheelEvent(newWheelEvent));

    FloatSize mergedDelta = oldNextWheelEvent->delta() + newWheelEvent.delta();
    FloatSize mergedWheelTicks = oldNextWheelEvent->wheelTicks() + newWheelEvent.wheelTicks();

    return adoptPtr(new WebWheelEvent(WebEvent::Wheel, newWheelEvent.position(), newWheelEvent.globalPosition(), mergedDelta, mergedWheelTicks, newWheelEvent.granularity(), newWheelEvent.modifiers(), newWheelEvent.timestamp()));
#else
    return adoptPtr(new WebWheelEvent(newWheelEvent));
#endif
}
