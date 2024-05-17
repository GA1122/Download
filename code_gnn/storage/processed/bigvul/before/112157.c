DictionaryValue* AppNotificationToValue(
    const sync_pb::AppNotification& proto) {
  DictionaryValue* value = new DictionaryValue();
  SET_STR(guid);
  SET_STR(app_id);
  SET_INT64(creation_timestamp_ms);
  SET_STR(title);
  SET_STR(body_text);
  SET_STR(link_url);
  SET_STR(link_text);
  return value;
}
