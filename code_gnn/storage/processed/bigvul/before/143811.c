void PersistentSampleMapRecords::Release(const void* user) {
  DCHECK_EQ(user_, user);
  user_ = nullptr;
}
