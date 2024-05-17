void HeadlessWebContentsImpl::SetBeginFrameEventsEnabled(int session_id,
                                                         bool enabled) {
  TRACE_EVENT2("headless",
               "HeadlessWebContentsImpl::SetBeginFrameEventsEnabled",
               "session_id", session_id, "enabled", enabled);

  if (enabled) {
    if (!base::ContainsValue(begin_frame_events_enabled_sessions_,
                             session_id)) {
      begin_frame_events_enabled_sessions_.push_back(session_id);

      if (needs_external_begin_frames_)
        SendNeedsBeginFramesEvent(session_id);
    }
  } else {
    begin_frame_events_enabled_sessions_.remove(session_id);
  }
}
