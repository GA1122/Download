DictionaryValue* CommitMessageToValue(
    const sync_pb::CommitMessage& proto,
    bool include_specifics) {
  DictionaryValue* value = new DictionaryValue();
  value->Set("entries",
             SyncEntitiesToValue(proto.entries(), include_specifics));
  SET_STR(cache_guid);
  SET_REP(extensions_activity, ChromiumExtensionActivityToValue);
  return value;
}
