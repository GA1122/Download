void ResourceTracker::SetSingletonOverride(ResourceTracker* tracker) {
  DCHECK(!singleton_override_);
  singleton_override_ = tracker;
}
