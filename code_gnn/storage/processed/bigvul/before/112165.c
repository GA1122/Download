DictionaryValue* ClientCommandToValue(const sync_pb::ClientCommand& proto) {
  DictionaryValue* value = new DictionaryValue();
  SET_INT32(set_sync_poll_interval);
  SET_INT32(set_sync_long_poll_interval);
  SET_INT32(max_commit_batch_size);
  SET_INT32(sessions_commit_delay_seconds);
  SET_INT32(throttle_delay_seconds);
  return value;
}
