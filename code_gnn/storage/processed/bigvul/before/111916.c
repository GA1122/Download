void ProfileSyncService::OnStopSyncingPermanently() {
  UpdateAuthErrorState(
      GoogleServiceAuthError(GoogleServiceAuthError::SERVICE_UNAVAILABLE));
  sync_prefs_.SetStartSuppressed(true);
  DisableForUser();
}
