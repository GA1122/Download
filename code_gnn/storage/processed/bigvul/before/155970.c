DiceTurnSyncOnHelper::GetProfileSyncService() {
  return profile_->IsSyncAllowed()
             ? ProfileSyncServiceFactory::GetForProfile(profile_)
             : nullptr;
}
