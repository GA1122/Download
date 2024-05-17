bool ChromePasswordManagerClient::IsSavingAndFillingEnabled(
    const GURL& url) const {
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kEnableAutomation)) {
    return false;
  }
  return *saving_and_filling_passwords_enabled_ && !IsIncognito() &&
         IsFillingEnabled(url);
}
