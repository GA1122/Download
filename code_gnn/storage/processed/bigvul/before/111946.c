void ProfileSyncService::UpdateAuthErrorState(
    const GoogleServiceAuthError& error) {
  is_auth_in_progress_ = false;
  last_auth_error_ = error;

  NotifyObservers();
}
