DictionaryValue* PreferenceSpecificsToValue(
    const sync_pb::PreferenceSpecifics& proto) {
  DictionaryValue* value = new DictionaryValue();
  SET_STR(name);
  SET_STR(value);
  return value;
}
