void InspectorNetworkAgent::DelayedRemoveReplayXHR(XMLHttpRequest* xhr) {
  if (!replay_xhrs_.Contains(xhr))
    return;
  replay_xhrs_to_be_deleted_.insert(xhr);
  replay_xhrs_.erase(xhr);
  remove_finished_replay_xhr_timer_.StartOneShot(0, BLINK_FROM_HERE);
}
