DictionaryValue* CommitResponseToValue(const sync_pb::CommitResponse& proto) {
  DictionaryValue* value = new DictionaryValue();
  SET_REP(entryresponse, EntryResponseToValue);
  return value;
}
