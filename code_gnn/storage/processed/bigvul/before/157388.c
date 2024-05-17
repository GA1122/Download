void HTMLMediaElement::ScheduleNotifyPlaying() {
  ScheduleEvent(EventTypeNames::playing);
  ScheduleResolvePlayPromises();
}
