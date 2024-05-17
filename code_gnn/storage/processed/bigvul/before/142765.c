 void HTMLMediaElement::ContextDestroyed(ExecutionContext*) {
   BLINK_MEDIA_LOG << "contextDestroyed(" << (void*)this << ")";
 
  CancelPendingEventsAndCallbacks();

  ClearMediaPlayer();
  ready_state_ = kHaveNothing;
  ready_state_maximum_ = kHaveNothing;
  SetNetworkState(kNetworkEmpty);
  SetShouldDelayLoadEvent(false);
  current_source_node_ = nullptr;
  official_playback_position_ = 0;
  official_playback_position_needs_update_ = true;
  GetCueTimeline().UpdateActiveCues(0);
  playing_ = false;
  paused_ = true;
  seeking_ = false;

  if (GetLayoutObject())
    GetLayoutObject()->UpdateFromElement();

  StopPeriodicTimers();
  removed_from_document_timer_.Stop();
}
