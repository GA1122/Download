DictionaryValue* TraceEventTestFixture::FindNamePhase(const char* name,
                                                      const char* phase) {
  JsonKeyValue key_values[] = {
    {"name", name, IS_EQUAL},
    {"ph", phase, IS_EQUAL},
    {0, 0, IS_EQUAL}
  };
  return FindMatchingTraceEntry(key_values);
}
