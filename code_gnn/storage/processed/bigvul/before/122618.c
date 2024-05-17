bool Extension::CheckPlatformAppFeatures(std::string* utf8_error) const {
  if (!is_platform_app())
    return true;

  if (!has_background_page()) {
    *utf8_error = errors::kBackgroundRequiredForPlatformApps;
    return false;
  }

  if (!incognito_split_mode_) {
    *utf8_error = errors::kInvalidIncognitoModeForPlatformApp;
    return false;
  }

  return true;
}
