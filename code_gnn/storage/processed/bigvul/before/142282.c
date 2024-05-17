const identity::IdentityManager* GetIdentityManagerForOriginalProfile(
    Profile* profile) {
  return IdentityManagerFactory::GetForProfile(profile->GetOriginalProfile());
}
