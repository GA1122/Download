bool ProfileSyncService::IsSyncTokenAvailable() {
  TokenService* token_service = TokenServiceFactory::GetForProfile(profile_);
  if (!token_service)
    return false;
  return token_service->HasTokenForService(GaiaConstants::kSyncService);
}
