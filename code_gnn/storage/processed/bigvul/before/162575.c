void Resource::SetLoader(ResourceLoader* loader) {
  CHECK(!loader_);
  DCHECK(StillNeedsLoad());
  loader_ = loader;
  status_ = ResourceStatus::kPending;
}
