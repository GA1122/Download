static Event* CreateEventWithTarget(const AtomicString& event_name,
                                    EventTarget* event_target) {
  Event* event = Event::Create(event_name);
  event->SetTarget(event_target);
  return event;
}
