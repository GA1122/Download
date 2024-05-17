bool ChromeNetworkDelegate::OnCanEnablePrivacyMode(
    const GURL& url,
    const GURL& first_party_for_cookies) const {
  if (!cookie_settings_.get())
    return false;

  bool reading_cookie_allowed = cookie_settings_->IsReadingCookieAllowed(
      url, first_party_for_cookies);
  bool setting_cookie_allowed = cookie_settings_->IsSettingCookieAllowed(
      url, first_party_for_cookies);
  bool privacy_mode = !(reading_cookie_allowed && setting_cookie_allowed);
  return privacy_mode;
}
