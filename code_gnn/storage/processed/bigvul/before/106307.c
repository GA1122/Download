void SyncBackendHost::Core::DoRequestNudge(
    const tracked_objects::Location& nudge_location) {
  syncapi_->RequestNudge(nudge_location);
}
