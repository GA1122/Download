DictionaryValue* DeviceInformationToValue(
    const sync_pb::DeviceInformation& proto) {
  DictionaryValue* value = new DictionaryValue();
  SET_STR(cache_guid);
  SET_STR(name);
  SET_STR(platform);
  SET_STR(chrome_version);
  return value;
}
