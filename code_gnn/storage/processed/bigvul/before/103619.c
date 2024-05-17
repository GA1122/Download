WebPreferences ChromeContentBrowserClient::GetWebkitPrefs(Profile* profile,
                                                          bool is_web_ui) {
  return RenderViewHostDelegateHelper::GetWebkitPrefs(profile, is_web_ui);
}
