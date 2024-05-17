void ChromeContentBrowserClient::UpdateInspectorSetting(
    RenderViewHost* rvh, const std::string& key, const std::string& value) {
  content::BrowserContext* browser_context =
      rvh->GetProcess()->GetBrowserContext();
  DictionaryPrefUpdate update(
      Profile::FromBrowserContext(browser_context)->GetPrefs(),
      prefs::kWebKitInspectorSettings);
  DictionaryValue* inspector_settings = update.Get();
  inspector_settings->SetWithoutPathExpansion(key,
                                              Value::CreateStringValue(value));
}
