void HTMLMediaElement::ScheduleEvent(const AtomicString& event_name) {
  Event* event = Event::CreateCancelable(event_name);
  event->SetTarget(this);
  ScheduleEvent(event);
}
