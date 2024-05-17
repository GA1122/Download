std::unique_ptr<base::DictionaryValue> SerializeTarget(
    scoped_refptr<DevToolsAgentHost> host) {
  std::unique_ptr<base::DictionaryValue> dictionary(
      new base::DictionaryValue());
  dictionary->SetString(kTargetIdField, host->GetId());
  dictionary->SetString(kTargetTitleField, host->GetTitle());
  dictionary->SetBoolean(kTargetAttachedField, host->IsAttached());
  dictionary->SetString(kTargetUrlField, host->GetURL().spec());

  std::string type = host->GetType();
  std::string target_type = kTargetTypeOther;
  if (type == DevToolsAgentHost::kTypePage) {
    int tab_id =
        extensions::ExtensionTabUtil::GetTabId(host->GetWebContents());
    dictionary->SetInteger(kTargetTabIdField, tab_id);
    target_type = kTargetTypePage;
  } else if (type == ChromeDevToolsManagerDelegate::kTypeBackgroundPage) {
    dictionary->SetString(kTargetExtensionIdField, host->GetURL().host());
    target_type = kTargetTypeBackgroundPage;
  } else if (type == DevToolsAgentHost::kTypeServiceWorker ||
             type == DevToolsAgentHost::kTypeSharedWorker) {
    target_type = kTargetTypeWorker;
  }

  dictionary->SetString(kTargetTypeField, target_type);

  GURL favicon_url = host->GetFaviconURL();
  if (favicon_url.is_valid())
    dictionary->SetString(kTargetFaviconUrlField, favicon_url.spec());

  return dictionary;
}
