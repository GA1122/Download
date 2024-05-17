bool ChromePasswordManagerClient::IsUnderAdvancedProtection() const {
#if defined(FULL_SAFE_BROWSING)
  return safe_browsing::AdvancedProtectionStatusManager::
      IsUnderAdvancedProtection(profile_);
#else
  return false;
#endif
}
