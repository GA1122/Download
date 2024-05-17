void HTMLMediaElement::ChangeNetworkStateFromLoadingToIdle() {
  progress_event_timer_.Stop();

  if (!MediaShouldBeOpaque()) {
    if (GetWebMediaPlayer() && GetWebMediaPlayer()->DidLoadingProgress())
      ScheduleEvent(EventTypeNames::progress);
    ScheduleEvent(EventTypeNames::suspend);
    SetNetworkState(kNetworkIdle);
  }
}
