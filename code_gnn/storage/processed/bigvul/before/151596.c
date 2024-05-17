void ResourceLoader::CancelTimerFired(TimerBase*) {
  if (loader_ && !resource_->HasClientsOrObservers())
    Cancel();
}
