  static CrostiniUpgrader* GetForProfile(Profile* profile) {
    return static_cast<CrostiniUpgrader*>(
        GetInstance()->GetServiceForBrowserContext(profile, true));
  }
