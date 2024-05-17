void ProfileSyncService::UnsuppressAndStart() {
  DCHECK(profile_);
  sync_prefs_.SetStartSuppressed(false);
  if (signin_ && signin_->GetAuthenticatedUsername().empty()) {
    signin_->SetAuthenticatedUsername(sync_prefs_.GetGoogleServicesUsername());
  }
  TryStart();
}
