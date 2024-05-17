DictionaryValue* DataTypeProgressMarkerToValue(
    const sync_pb::DataTypeProgressMarker& proto) {
  DictionaryValue* value = new DictionaryValue();
  SET_INT32(data_type_id);
  SET_BYTES(token);
  SET_INT64(timestamp_token_for_migration);
  SET_STR(notification_hint);
  return value;
}
