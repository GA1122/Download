void HTMLMediaElement::ScheduleEvent(const AtomicString& event_name) {
  ScheduleEvent(Event::CreateCancelable(event_name));
}
