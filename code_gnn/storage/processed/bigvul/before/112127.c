void SyncManager::SyncInternal::StoreState(
    const std::string& state) {
  if (!directory()) {
    LOG(ERROR) << "Could not write notification state";
    return;
  }
  if (VLOG_IS_ON(1)) {
    std::string encoded_state;
    base::Base64Encode(state, &encoded_state);
    DVLOG(1) << "Writing notification state: " << encoded_state;
  }
  directory()->SetNotificationState(state);
  directory()->SaveChanges();
}
