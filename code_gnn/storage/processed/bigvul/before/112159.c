DictionaryValue* AppSettingsToValue(
    const sync_pb::AppNotificationSettings& proto) {
  DictionaryValue* value = new DictionaryValue();
  SET_BOOL(initial_setup_done);
  SET_BOOL(disabled);
  SET_STR(oauth_client_id);
  return value;
}
