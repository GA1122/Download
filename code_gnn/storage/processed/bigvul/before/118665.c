    UserCloudPolicyManagerFactoryChromeOS::GetManagerForProfile(
        Profile* profile) {
  ManagerMap::const_iterator it = managers_.find(profile->GetOriginalProfile());
  return it != managers_.end() ? it->second : NULL;
}
