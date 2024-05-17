base::Value* SerializeTarget(const DevToolsTargetImpl& target) {
  base::DictionaryValue* dictionary = new base::DictionaryValue();

  dictionary->SetString(kTargetIdField, target.GetId());
  dictionary->SetString(kTargetTitleField, target.GetTitle());
  dictionary->SetBoolean(kTargetAttachedField, target.IsAttached());
  dictionary->SetString(kTargetUrlField, target.GetURL().spec());

  std::string type = target.GetType();
  if (type == kTargetTypePage) {
    dictionary->SetInteger(kTargetTabIdField, target.GetTabId());
  } else if (type == kTargetTypeBackgroundPage) {
    dictionary->SetString(kTargetExtensionIdField, target.GetExtensionId());
  } else if (type != kTargetTypeWorker) {
    type = kTargetTypeOther;
  }
  dictionary->SetString(kTargetTypeField, type);

  GURL favicon_url = target.GetFaviconURL();
  if (favicon_url.is_valid())
    dictionary->SetString(kTargetFaviconUrlField, favicon_url.spec());

  return dictionary;
}
