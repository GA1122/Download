void ServiceWorkerContextCore::RemoveLiveVersion(int64_t id) {
  if (live_versions_[id]->running_status() != EmbeddedWorkerStatus::STOPPED) {
    observer_list_->Notify(
        FROM_HERE, &ServiceWorkerContextCoreObserver::OnRunningStateChanged, id,
        EmbeddedWorkerStatus::STOPPED);
  }

  live_versions_.erase(id);
}
