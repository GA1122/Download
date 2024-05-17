DictionaryValue* BookmarkSpecificsToValue(
    const sync_pb::BookmarkSpecifics& proto) {
  DictionaryValue* value = new DictionaryValue();
  SET_STR(url);
  SET_BYTES(favicon);
  SET_STR(title);
  return value;
}
