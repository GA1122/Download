int BackendImpl::SelfCheck() {
  if (!init_) {
    LOG(ERROR) << "Init failed";
    return ERR_INIT_FAILED;
  }

  int num_entries = rankings_.SelfCheck();
  if (num_entries < 0) {
    LOG(ERROR) << "Invalid rankings list, error " << num_entries;
#if !defined(NET_BUILD_STRESS_CACHE)
    return num_entries;
#endif
  }

  if (num_entries != data_->header.num_entries) {
    LOG(ERROR) << "Number of entries mismatch";
#if !defined(NET_BUILD_STRESS_CACHE)
    return ERR_NUM_ENTRIES_MISMATCH;
#endif
  }

  return CheckAllEntries();
}
