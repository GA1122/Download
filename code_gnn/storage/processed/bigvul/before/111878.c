SyncCredentials ProfileSyncService::GetCredentials() {
  SyncCredentials credentials;
  credentials.email = signin_->GetAuthenticatedUsername();
  DCHECK(!credentials.email.empty());
  TokenService* service = TokenServiceFactory::GetForProfile(profile_);
  if (service->HasTokenForService(GaiaConstants::kSyncService)) {
      credentials.sync_token = service->GetTokenForService(
          GaiaConstants::kSyncService);
    UMA_HISTOGRAM_BOOLEAN("Sync.CredentialsLost", false);
  } else {
    UMA_HISTOGRAM_BOOLEAN("Sync.CredentialsLost", true);
    credentials.sync_token = "credentials_lost";
  }
  return credentials;
}
