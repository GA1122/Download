const DictionaryValue* FindTraceEntry(
    const ListValue& trace_parsed,
    const char* string_to_match,
    const DictionaryValue* match_after_this_item = NULL) {
  size_t trace_parsed_count = trace_parsed.GetSize();
  for (size_t i = 0; i < trace_parsed_count; i++) {
    const Value* value = NULL;
    trace_parsed.Get(i, &value);
    if (match_after_this_item) {
      if (value == match_after_this_item)
         match_after_this_item = NULL;
      continue;
    }
    if (!value || value->GetType() != Value::TYPE_DICTIONARY)
      continue;
    const DictionaryValue* dict = static_cast<const DictionaryValue*>(value);

    if (IsStringInDict(string_to_match, dict))
      return dict;
  }
  return NULL;
}
