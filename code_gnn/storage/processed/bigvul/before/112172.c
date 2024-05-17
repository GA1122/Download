DictionaryValue* EncryptedDataToValue(const sync_pb::EncryptedData& proto) {
  DictionaryValue* value = new DictionaryValue();
  SET_STR(key_name);
  SET_BYTES(blob);
  return value;
}
