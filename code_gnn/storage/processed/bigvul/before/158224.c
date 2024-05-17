void AppCacheGroup::QueueUpdate(AppCacheHost* host,
                                const GURL& new_master_resource) {
  DCHECK(update_job_ && host && !new_master_resource.is_empty());
  queued_updates_.insert(QueuedUpdates::value_type(
      host, std::make_pair(host, new_master_resource)));

  host->AddObserver(host_observer_.get());

  if (FindObserver(host, observers_)) {
    observers_.RemoveObserver(host);
    queued_observers_.AddObserver(host);
  }
}
