PersistentMemoryAllocator::Reference PersistentSampleMapRecords::GetNext() {
  DCHECK(user_);

  if (records_.size() == seen_) {
    if (!data_manager_->LoadRecords(this))
      return false;
  }

  DCHECK_LT(seen_, records_.size());
  return records_[seen_++];
}
