bool ProfileSyncService::waiting_for_auth() const {
  return is_auth_in_progress_;
}
