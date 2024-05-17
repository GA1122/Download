void AppCacheGroup::RemoveUpdateObserver(UpdateObserver* observer) {
  observers_.RemoveObserver(observer);
  queued_observers_.RemoveObserver(observer);
}
