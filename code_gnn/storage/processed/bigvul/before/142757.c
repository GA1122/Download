void HTMLMediaElement::ChangeNetworkStateFromLoadingToIdle() {
  progress_event_timer_.Stop();

  if (!MediaShouldBeOpaque()) {
    if (GetWebMediaPlayer() && GetWebMediaPlayer()->DidLoadingProgress())
      ScheduleEvent(event_type_names::kProgress);
    ScheduleEvent(event_type_names::kSuspend);
    SetNetworkState(kNetworkIdle);
  }
}
