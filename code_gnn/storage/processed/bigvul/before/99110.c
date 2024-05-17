bool ResourceDispatcherHost::PauseRequestIfNeeded(
    ResourceDispatcherHostRequestInfo* info) {
  if (info->pause_count() > 0)
    info->set_is_paused(true);
  return info->is_paused();
}
