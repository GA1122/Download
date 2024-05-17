void ResourceTracker::ClearSingletonOverride() {
  DCHECK(singleton_override_);
  singleton_override_ = NULL;
}
