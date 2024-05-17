void AppCacheGroup::HostDestructionImminent(AppCacheHost* host) {
  queued_updates_.erase(host);
  if (queued_updates_.empty() && !restart_update_task_.IsCancelled())
    restart_update_task_.Cancel();
}
