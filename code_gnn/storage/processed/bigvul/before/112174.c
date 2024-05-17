DictionaryValue* EntryResponseToValue(
    const sync_pb::CommitResponse::EntryResponse& proto) {
  DictionaryValue* value = new DictionaryValue();
  SET_ENUM(response_type, GetResponseTypeString);
  SET_STR(id_string);
  SET_STR(parent_id_string);
  SET_INT64(position_in_parent);
  SET_INT64(version);
  SET_STR(name);
  SET_STR(error_message);
  SET_INT64(mtime);
  return value;
}
