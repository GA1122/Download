DictionaryValue* AppSettingSpecificsToValue(
    const sync_pb::AppSettingSpecifics& proto) {
  DictionaryValue* value = new DictionaryValue();
  SET(extension_setting, ExtensionSettingSpecificsToValue);
  return value;
}
