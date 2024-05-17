AppCache::~AppCache() {
  DCHECK(associated_hosts_.empty());
  if (owning_group_.get()) {
    DCHECK(is_complete_);
    owning_group_->RemoveCache(this);
  }
  DCHECK(!owning_group_.get());
  storage_->working_set()->RemoveCache(this);
}
