void ChromeContentBrowserClient::SetDefaultQuotaSettingsForTesting(
    const storage::QuotaSettings* settings) {
  g_default_quota_settings = settings;
}
