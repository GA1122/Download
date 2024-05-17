void AppCacheGroup::AddUpdateObserver(UpdateObserver* observer) {
  if (queued_updates_.find(observer) != queued_updates_.end())
    queued_observers_.AddObserver(observer);
  else
    observers_.AddObserver(observer);
}
