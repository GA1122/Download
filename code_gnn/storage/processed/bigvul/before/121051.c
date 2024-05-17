Value* NetLogSendRequestBodyCallback(int length,
                                     bool is_chunked,
                                     bool did_merge,
                                     net::NetLog::LogLevel  ) {
  DictionaryValue* dict = new DictionaryValue();
  dict->SetInteger("length", length);
  dict->SetBoolean("is_chunked", is_chunked);
  dict->SetBoolean("did_merge", did_merge);
  return dict;
}
