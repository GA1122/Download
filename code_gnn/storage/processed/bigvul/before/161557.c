GoogleChromeDistribution::GoogleChromeDistribution()
    : BrowserDistribution(std::make_unique<UpdatingAppRegistrationData>(
          install_static::GetAppGuid())) {}
