  chromeos::NoteTakingLockScreenSupport GetAppLockScreenStatus(
      const std::string& app_id) {
    std::unique_ptr<chromeos::NoteTakingAppInfo> info =
        chromeos::NoteTakingHelper::Get()->GetPreferredChromeAppInfo(
            browser()->profile());
    if (!info || info->app_id != app_id)
      return chromeos::NoteTakingLockScreenSupport::kNotSupported;
    return info->lock_screen_support;
  }
