void AppCacheGroup::RunQueuedUpdates() {
  if (!restart_update_task_.IsCancelled())
    restart_update_task_.Cancel();

  if (queued_updates_.empty())
    return;

  QueuedUpdates updates_to_run;
  queued_updates_.swap(updates_to_run);
  DCHECK(queued_updates_.empty());

  for (auto& pair : updates_to_run) {
    AppCacheHost* host = pair.second.first;
    host->RemoveObserver(host_observer_.get());
    if (FindObserver(host, queued_observers_)) {
      queued_observers_.RemoveObserver(host);
      observers_.AddObserver(host);
    }

    if (!is_obsolete() && !is_being_deleted())
      StartUpdateWithNewMasterEntry(host, pair.second.second);
  }
}
