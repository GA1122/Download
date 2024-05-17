bool TraceEventTestFixture::FindMatchingValue(const char* key,
                                              const char* value) {
  JsonKeyValue key_values[] = {
    {key, value, IS_EQUAL},
    {0, 0, IS_EQUAL}
  };
  return FindMatchingTraceEntry(key_values);
}