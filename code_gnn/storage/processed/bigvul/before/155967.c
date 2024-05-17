void DiceTurnSyncOnHelper::EnableUnifiedConsentIfNeeded() {
  if (unified_consent::IsUnifiedConsentFeatureEnabled()) {
    UnifiedConsentServiceFactory::GetForProfile(profile_)
        ->SetUnifiedConsentGiven(true);
  }
}
