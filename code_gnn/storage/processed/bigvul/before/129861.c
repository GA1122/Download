bool IsStringInDict(const char* string_to_match, const DictionaryValue* dict) {
  for (DictionaryValue::Iterator it(*dict); !it.IsAtEnd(); it.Advance()) {
    if (it.key().find(string_to_match) != std::string::npos)
      return true;

    std::string value_str;
    it.value().GetAsString(&value_str);
    if (value_str.find(string_to_match) != std::string::npos)
      return true;
  }

  const DictionaryValue* args_dict = NULL;
  dict->GetDictionary("args", &args_dict);
  if (args_dict)
    return IsStringInDict(string_to_match, args_dict);

  return false;
}
