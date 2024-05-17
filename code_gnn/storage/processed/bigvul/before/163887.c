void DevToolsEventForwarder::SetWhitelistedShortcuts(
    const std::string& message) {
  std::unique_ptr<base::Value> parsed_message = base::JSONReader::Read(message);
  base::ListValue* shortcut_list;
  if (!parsed_message || !parsed_message->GetAsList(&shortcut_list))
      return;
  base::ListValue::iterator it = shortcut_list->begin();
  for (; it != shortcut_list->end(); ++it) {
    base::DictionaryValue* dictionary;
    if (!it->GetAsDictionary(&dictionary))
      continue;
    int key_code = 0;
    dictionary->GetInteger("keyCode", &key_code);
    if (key_code == 0)
      continue;
    int modifiers = 0;
    dictionary->GetInteger("modifiers", &modifiers);
    if (!KeyWhitelistingAllowed(key_code, modifiers)) {
      LOG(WARNING) << "Key whitelisting forbidden: "
                   << "(" << key_code << "," << modifiers << ")";
      continue;
    }
    whitelisted_keys_.insert(CombineKeyCodeAndModifiers(key_code, modifiers));
  }
}
