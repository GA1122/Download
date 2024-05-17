void CloudPolicyCacheBase::SetUnmanagedInternal(const base::Time& timestamp) {
  is_unmanaged_ = true;
  initialization_complete_ = true;
  public_key_version_.valid = false;
  mandatory_policy_.Clear();
  recommended_policy_.Clear();
  last_policy_refresh_time_ = timestamp;

  FOR_EACH_OBSERVER(Observer, observer_list_, OnCacheUpdate(this));
}
