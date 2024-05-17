CloudPolicyCacheBase::CloudPolicyCacheBase()
    : notifier_(NULL),
      initialization_complete_(false),
      is_unmanaged_(false) {
  public_key_version_.version = 0;
  public_key_version_.valid = false;
}
