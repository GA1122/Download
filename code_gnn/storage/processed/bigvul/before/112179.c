DictionaryValue* GetUpdatesMessageToValue(
    const sync_pb::GetUpdatesMessage& proto) {
  DictionaryValue* value = new DictionaryValue();
  SET(caller_info, GetUpdatesCallerInfoToValue);
  SET_BOOL(fetch_folders);
  SET_INT32(batch_size);
  SET_REP(from_progress_marker, DataTypeProgressMarkerToValue);
  SET_BOOL(streaming);
  SET_BOOL(create_mobile_bookmarks_folder);
  return value;
}
