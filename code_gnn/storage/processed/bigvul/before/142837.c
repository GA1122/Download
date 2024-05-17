void HTMLMediaElement::MediaEngineError(MediaError* err) {
  DCHECK_GE(ready_state_, kHaveMetadata);
  BLINK_MEDIA_LOG << "mediaEngineError(" << (void*)this << ", "
                  << static_cast<int>(err->code()) << ")";

  StopPeriodicTimers();
  load_state_ = kWaitingForSource;

  error_ = err;

  ScheduleEvent(event_type_names::kError);

  SetNetworkState(kNetworkIdle);

  SetShouldDelayLoadEvent(false);

  current_source_node_ = nullptr;
}
