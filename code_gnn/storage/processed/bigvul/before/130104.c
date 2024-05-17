void MasterPreferences::InitializeProductFlags() {
  multi_install_ = false;
  chrome_ = true;

  GetBool(installer::master_preferences::kMultiInstall, &multi_install_);

  if (multi_install_) {
    if (!GetBool(installer::master_preferences::kChrome, &chrome_))
      chrome_ = false;
  } else {
    chrome_ = true;
  }
}
