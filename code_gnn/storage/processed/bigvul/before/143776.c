PersistentSampleMapRecords* PersistentSampleMapRecords::Acquire(
    const void* user) {
  DCHECK(!user_);
  user_ = user;
  seen_ = 0;
  return this;
}
