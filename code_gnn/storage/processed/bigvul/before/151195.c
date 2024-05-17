void InspectorNetworkAgent::RemoveFinishedReplayXHRFired(TimerBase*) {
  replay_xhrs_to_be_deleted_.clear();
}
