DictionaryValue* ErrorToValue(
    const sync_pb::ClientToServerResponse::Error& proto) {
  DictionaryValue* value = new DictionaryValue();
  SET_ENUM(error_type, GetErrorTypeString);
  SET_STR(error_description);
  SET_STR(url);
  SET_ENUM(action, GetActionString);
  return value;
}
