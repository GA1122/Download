void HTMLMediaElement::ScheduleNotifyPlaying() {
  ScheduleEvent(event_type_names::kPlaying);
  ScheduleResolvePlayPromises();
}
